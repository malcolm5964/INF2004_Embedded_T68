#include "magnometer.h"

//i2c Pins
const uint sda_pin = 16;
const uint scl_pin = 17;

//Ports
#define I2C_PORT i2c0

uint8_t magData[6];
uint8_t accData[6];

//Magnometer Register and Values
const uint8_t MAG_ADDRESS = 0x1E;

const int CRA_REG_M_ADDR = 0x00;
const int CRB_REG_M_ADDR = 0x01;
const int MR_REG_M_ADDR = 0x02;

int CRA_REG_M_value = 0x0C;
int CRB_REG_M_value = 0x10; //Gauss strength
int MR_REG_M_value = 0x00;

const uint8_t MAG_OUT_X_H_M = 0x03;

//Accelerator Register and Values
const uint8_t ACC_ADDRESS = 0x19;

const uint8_t CTRL_REG1_ADDR = 0x20;
const uint8_t CTRL_REG1_ADDR_value = 0x57;


volatile float headingPass;

typedef struct {
    int32_t x, y, z;
} vector_int32_t;

typedef struct {
    int16_t x, y, z;
} vector_int16_t;

typedef struct {
    int8_t x, y, z;
} vector_int8_t;

typedef struct {
    float x, y, z;
} vector_float_t;

void vector_cross_int16_float(const vector_int16_t *a, const vector_float_t *b, vector_float_t *out) {
    out->x = (float)(a->y * b->z - a->z * b->y);
    out->y = (float)(a->z * b->x - a->x * b->z);
    out->z = (float)(a->x * b->y - a->y * b->x);
}

// Heading calculation functions
void vector_cross_int32_int16(const vector_int32_t *a, const vector_int16_t *b, vector_float_t *out) {
    out->x = (float)(a->y * b->z - a->z * b->y);
    out->y = (float)(a->z * b->x - a->x * b->z);
    out->z = (float)(a->x * b->y - a->y * b->x);
}

void vector_normalize(vector_float_t *a) {
    float mag = sqrtf(a->x * a->x + a->y * a->y + a->z * a->z);
    a->x /= mag;
    a->y /= mag;
    a->z /= mag;
}

float vector_dot(const vector_float_t *a, const vector_int8_t *b) {
    return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

void magnometer_init(){
    printf("magno init\n");

    //Initialize I2C port at 400 kHz
    i2c_init(I2C_PORT, 400 * 1000);

    // Set I2C mode and slave address for magnetometer
    i2c_set_slave_mode(I2C_PORT, false, MAG_ADDRESS);

    sleep_ms(1000);

    // Enable pull-up resistors for SDA and SCL lines
    gpio_set_pulls(sda_pin, true, false);
    gpio_set_pulls(scl_pin, true, false);

    //Set SDA and SCL pin to I2C
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);

    //Enable Magnetometer
    uint8_t config[2] = {CRA_REG_M_ADDR, CRA_REG_M_value};
    i2c_write_blocking(I2C_PORT, MAG_ADDRESS, config, 2, true);

    //Change strength
    config[0] = CRB_REG_M_ADDR;
    config[1] = CRB_REG_M_value;
    i2c_write_blocking(I2C_PORT, MAG_ADDRESS, config, 2, true);
    
    //Enable continuous conversion
    config[0] = MR_REG_M_ADDR;
    config[1] = MR_REG_M_value;
    i2c_write_blocking(I2C_PORT, MAG_ADDRESS, config, 2, true);

    //Enable acceleratometer
    config[0] = CTRL_REG1_ADDR;
    config[1] = CTRL_REG1_ADDR_value;
    i2c_write_blocking(i2c0, ACC_ADDRESS, config, sizeof(config), true);
}


//Read Magnometer Data
float magnometer_read(){
    while(1) {
        //Magnometer read
        i2c_write_blocking(i2c0, MAG_ADDRESS, &MAG_OUT_X_H_M, 1, false);
        
        int result = i2c_read_blocking(i2c0, MAG_ADDRESS, magData, 6, false);
            if (result == PICO_ERROR_GENERIC) {
                printf("I2C read error\n");
                continue;
            }

        const uint8_t OUT_X_L_A = 0x28 | 0x80;
        //Accelerator read
        i2c_write_blocking(I2C_PORT, ACC_ADDRESS, &OUT_X_L_A, 1, true);
        i2c_read_blocking(I2C_PORT, ACC_ADDRESS, accData, 6, true);

        uint8_t xhm = magData[0];
        uint8_t xlm = magData[1];
        uint8_t zhm = magData[2];
        uint8_t zlm = magData[3];
        uint8_t yhm = magData[4];
        uint8_t ylm = magData[5];

        int16_t x_acc = (int16_t)((accData[1] << 8) | accData[0]) >> 4;
        int16_t y_acc = (int16_t)((accData[3] << 8) | accData[2]) >> 4;
        int16_t z_acc = (int16_t)((accData[5] << 8) | accData[4]) >> 4;

        //printf("X: %i, Y: %i, Z: %i\n", x_acc, y_acc, z_acc);

        int raw_xm = (int16_t)((xhm << 8) | xlm);
        int raw_ym = (int16_t)((yhm << 8) | ylm);
        int raw_zm = (int16_t)((zhm << 8) | zlm);

        //Calibration 
        //static int16_t minX = 0, maxX = 0, minY = 0, maxY = 0;
        //if (raw_xm < minX) minX = raw_xm;
        //if (raw_xm > maxX) maxX = raw_xm;
        //if (raw_ym < minY) minY = raw_ym;
        //if (raw_ym > maxY) maxY = raw_ym;
        //printf("minX: %d maxX: %d minY: %d maxY: %d\n", minX, maxX, minY, maxY);
        int16_t minX = -356;
        int16_t maxX = 536;
        int16_t minY = -652;
        int16_t maxY = 248;

        //int16_t xOffset = (minX + maxX) / 2;
        //int16_t yOffset = (minY + maxY) / 2;
//
//
        //int16_t x_calibrated = raw_xm - xOffset;
        //int16_t y_calibrated = raw_ym - yOffset;

        //printf("X: %i, Y: %i, Z: %i\n", raw_xm, raw_ym, raw_zm);

        vector_int32_t temp_m = {raw_xm, raw_ym, raw_zm};

        temp_m.x -= (minX + maxX) / 2;
        temp_m.y -= (minY + maxY) / 2;

        vector_int8_t from = {0, 1, 0}; 
        vector_int16_t acc = {x_acc,y_acc,z_acc};

        vector_float_t E, N;

        vector_cross_int32_int16(&temp_m, &acc, &E);
        vector_normalize(&E);
        vector_cross_int16_float(&acc, &E, &N);
        vector_normalize(&N);

        float heading = atan2f(vector_dot(&E, &from), vector_dot(&N, &from)) * 180.0f / M_PI;

        if (heading < 0.0f) {
            heading += 360.0f;
        }

        headingPass = heading;



        printf("Heading: %f degrees\n", heading);

        //sleep_ms(1000);
    }

    return 1;
}


