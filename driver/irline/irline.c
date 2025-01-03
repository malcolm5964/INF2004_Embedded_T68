#include "irline.h"

const uint LEFT_IR_PIN_ADC = 28;
const uint RIGHT_IR_PIN_ADC = 27;
const uint CENTER_IR_PIN_ADC = 26;

const uint LEFT_IR_SENSOR_GND = 22;
//CENTER_IR_PIN_GND = PIN 33
//RIGHT_IR_PIN_GND = PIN 28

const uint CENTER_COLOUR_THRESHOLD = 200;
const uint RIGHT_COLOUR_THRESHOLD = 1500;
const uint LEFT_COLOUR_THRESHOLD = 1500;

volatile uint32_t left_ir_raw;
volatile uint32_t right_ir_raw;
volatile uint32_t center_ir_raw;

volatile bool colour_detected;

void init_ir()
{
    adc_init();

    gpio_init(LEFT_IR_SENSOR_GND);
    gpio_set_dir(LEFT_IR_SENSOR_GND, GPIO_OUT);
    gpio_put(LEFT_IR_SENSOR_GND, 0); // Set to GND

    gpio_set_dir(LEFT_IR_PIN_ADC, GPIO_IN);
    gpio_set_function(LEFT_IR_PIN_ADC, GPIO_FUNC_SIO);
    gpio_disable_pulls(LEFT_IR_PIN_ADC);
    gpio_set_input_enabled(LEFT_IR_PIN_ADC, false);

    gpio_set_dir(RIGHT_IR_PIN_ADC, GPIO_IN);
    gpio_set_function(RIGHT_IR_PIN_ADC, GPIO_FUNC_SIO);
    gpio_disable_pulls(RIGHT_IR_PIN_ADC);
    gpio_set_input_enabled(RIGHT_IR_PIN_ADC, false);

    gpio_set_dir(CENTER_IR_PIN_ADC, GPIO_IN);
    gpio_set_function(CENTER_IR_PIN_ADC, GPIO_FUNC_SIO);
    gpio_disable_pulls(CENTER_IR_PIN_ADC);
    gpio_set_input_enabled(CENTER_IR_PIN_ADC, false);
}

void get_ir_value()
{
    while (true)
    {
        vTaskDelay(1000);
        adc_select_input(2);
        left_ir_raw = adc_read();

        adc_select_input(1);
        right_ir_raw = adc_read();

        adc_select_input(0);
        center_ir_raw = adc_read();

        //printf("center: %d\n", center_ir_raw);
        printf("right: %d\n", right_ir_raw);
        //printf("left: %d\n", left_ir_raw);
    }
}

bool left_ir()
{
    adc_select_input(2);
    left_ir_raw = adc_read();

    return left_ir_raw > LEFT_COLOUR_THRESHOLD;
}

bool right_ir()
{
    adc_select_input(1); 
    right_ir_raw = adc_read();

    return right_ir_raw > RIGHT_COLOUR_THRESHOLD;
}

bool front_ir()
{
    adc_select_input(0); 
    center_ir_raw = adc_read();

    return center_ir_raw > CENTER_COLOUR_THRESHOLD;
}