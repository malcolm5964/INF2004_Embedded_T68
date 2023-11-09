#include "wheelEncoder.h"

//Wheel IR Sensor
const uint WHEEL_EN_LEFT_OUT = 2;
const uint WHEEL_EN_LEFT_VCC = 3;
const uint WHEEL_EN_LEFT_GND = 4;

const uint WHEEL_EN_RIGHT_OUT = 0;
const uint WHEEL_EN_RIGHT_VCC = 1;

void initWheelEncoderLeft(){
    printf("Wheel encoder left init\n");
    // Init Left IR Sensor out
    gpio_init(WHEEL_EN_LEFT_OUT);
    gpio_set_dir(WHEEL_EN_LEFT_OUT, GPIO_IN);
    // Set to HIGH for VCC
    gpio_init(WHEEL_EN_LEFT_VCC);
    gpio_set_dir(WHEEL_EN_LEFT_VCC, GPIO_OUT);
    gpio_put(WHEEL_EN_LEFT_VCC, 1);
    //Set low for GND
    gpio_init(WHEEL_EN_LEFT_GND);
    gpio_set_dir(WHEEL_EN_LEFT_GND, GPIO_OUT);
    gpio_put(WHEEL_EN_LEFT_GND, 0);
}

void initWheelEncoderRight(){
    printf("Wheel encoder Right init\n");
    // Init Right IR Sensor
    gpio_init(WHEEL_EN_RIGHT_OUT);
    gpio_set_dir(WHEEL_EN_RIGHT_OUT, GPIO_IN);
    gpio_init(WHEEL_EN_RIGHT_VCC);
    gpio_set_dir(WHEEL_EN_RIGHT_VCC, GPIO_OUT);
    gpio_put(WHEEL_EN_RIGHT_VCC, 1); // Set to HIGH for VCC
}

volatile int currentSpeedLeft;
volatile int currentSpeedRight;

uint64_t measureSpeedRight(){
    absolute_time_t previousTime = get_absolute_time();
    uint8_t detect_change;

    while (true)
    {
        if((absolute_time_diff_us(previousTime, get_absolute_time()) > 1000000))
        {
            currentSpeedRight = 0;
            printf("Wheel Speed Right: %imm/s\n", currentSpeedRight);
        }
        if (gpio_get(WHEEL_EN_RIGHT_OUT) == 1)
        {
            detect_change = 1;
        }
        if ((gpio_get(WHEEL_EN_RIGHT_OUT) == 0) && (detect_change == 1))
        {
            detect_change = 0;

            //Add distance (Circumference = 20.4cm)

            absolute_time_t current_time = get_absolute_time();
            int64_t time = absolute_time_diff_us(previousTime, current_time);

            //Speed (mm/s) = (Distance (10mm) * 1,000,000) / Time (microseconds)
            int speed = 10000000 / time;
            if(speed > 1000)
            {
                currentSpeedRight = 0;
            }
            else
            {
                currentSpeedRight = speed;
                printf("Wheel Speed Right: %imm/s\n", speed);
                previousTime = current_time;
            }
        }
    }
}


uint64_t measureSpeedLeft(){
    absolute_time_t previousTime = get_absolute_time();
    uint8_t detect_change;

    while (true)
    {
        if((absolute_time_diff_us(previousTime, get_absolute_time()) > 1000000))
        {
            currentSpeedLeft = 0;
            //printf("Wheel Speed left: %lldmm/s\n", currentSpeedLeft);
        }
        if (gpio_get(WHEEL_EN_LEFT_OUT) == 1)
        {
            detect_change = 1;
        }
        if ((gpio_get(WHEEL_EN_LEFT_OUT) == 0) && (detect_change == 1))
        {
            detect_change = 0;

            //Add distance (Circumference = 20.4cm)

            absolute_time_t current_time = get_absolute_time();
            uint64_t time = absolute_time_diff_us(previousTime, current_time);

            //Speed (cm/s) = (Distance (10mm) * 1,000,000) / Time (microseconds)
            int speed = 10000000 / time;
            if(speed > 1000)
            {
                currentSpeedLeft = 0;
            }
            else
            {
                currentSpeedLeft = speed;
                printf("Wheel Speed left: %imm/s\n", speed);
                previousTime = current_time;
            }
        }
        
    }
}