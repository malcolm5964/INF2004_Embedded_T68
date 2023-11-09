#include "motor.h"

const uint PWM_LEFT = 10;
const uint N1 = 11;
const uint N2 = 12;

const uint PWM_RIGHT = 15;
const uint N3 = 14;
const uint N4 = 13;

uint slice_num_right = 0;
uint slice_num_left = 0;

void set_direction_forward()
{   
    gpio_put(N1, 0);
    gpio_put(N2, 1);
    gpio_put(N3, 1);
    gpio_put(N4, 0);
}

void set_direction_back()
{
    gpio_put(N1, 1);
    gpio_put(N2, 0);    
    gpio_put(N3, 0);
    gpio_put(N4, 1);
}

void set_direction_right()
{
    gpio_put(N1, 0);
    gpio_put(N2, 1);    
    gpio_put(N3, 0);
    gpio_put(N4, 0);
}

void set_direction_left()
{
    gpio_put(N1, 0);
    gpio_put(N2, 0);    
    gpio_put(N3, 1);
    gpio_put(N4, 0);
}

void stop()
{
    gpio_put(N1, 0);
    gpio_put(N2, 0);    
    gpio_put(N3, 0);
    gpio_put(N4, 0);
}

void set_left_speed(int PWM){
    pwm_set_chan_level(slice_num_left, PWM_CHAN_A, PWM);
}

void set_right_speed(int PWM){
    pwm_set_chan_level(slice_num_right, PWM_CHAN_B, PWM);
}


void init_right_motor(){
    printf("Init All\n");
    gpio_set_function(PWM_RIGHT, GPIO_FUNC_PWM);
    slice_num_right = pwm_gpio_to_slice_num(PWM_RIGHT);
    pwm_set_clkdiv(slice_num_right, 100);
    pwm_set_wrap(slice_num_right, 50000);
    pwm_set_enabled(slice_num_right, true);
    gpio_init(N3);
    gpio_init(N4);
    gpio_set_dir(N3, GPIO_OUT);
    gpio_set_dir(N4, GPIO_OUT);
}

void init_left_motor(){
    gpio_set_function(PWM_LEFT, GPIO_FUNC_PWM);
    slice_num_left = pwm_gpio_to_slice_num(PWM_LEFT);
    pwm_set_clkdiv(slice_num_left, 100);
    pwm_set_wrap(slice_num_left, 50000);
    pwm_set_enabled(slice_num_left, true);
    gpio_init(N1);
    gpio_init(N2);
    gpio_set_dir(N1, GPIO_OUT);
    gpio_set_dir(N2, GPIO_OUT);
}


int max_pwm = 50000; //MAx PWM = 50000
double kp = 0.05; //kp = 10
int16_t current_PWM_left;
int16_t current_PWM_right;

void move_forward_left(int target_speed, int current_speed){

    set_direction_forward();

    printf("LEFT Target Speed: %i Current Speed: %i\n", target_speed, current_speed);
        
    //Calculate error
    double error = target_speed - current_speed;

    //Calculate the Control Variable
    double proportional = kp * error;

    int16_t PWM = (int16_t)(proportional / 500 * max_pwm);

    current_PWM_left += PWM;

    printf("LEFT PWM: %i\n", current_PWM_left);

    // Apply limits to the control variable
    if (current_PWM_left > max_pwm) {
        current_PWM_left = max_pwm;
    } else if (current_PWM_left < 10000) {
        current_PWM_left = 10000;
    }

    set_left_speed(current_PWM_left);
}

void move_forward_right(int target_speed, int current_speed){

    set_direction_forward();

    printf("RIGHT Target Speed: %i Current Speed: %i\n", target_speed, current_speed);
        
    //Calculate error
    double error = target_speed - current_speed;

    //Calculate the Control Variable
    double proportional = kp * error;

    int16_t PWM = (int16_t)(proportional / 500 * max_pwm);

    current_PWM_right += PWM;

    printf("RIGHT PWM: %i\n", current_PWM_right);

    // Apply limits to the control variable
    if (current_PWM_right > max_pwm) {
        current_PWM_right = max_pwm;
    } else if (current_PWM_right < 10000) {
        current_PWM_right = 10000;
    }

    set_right_speed(current_PWM_right);
}

void move_backward(){
    set_direction_back();
    set_left_speed(3);
    set_left_speed(3);
}

void turn_right(){
    set_direction_right();
    set_left_speed(3);
    vTaskDelay(950);
}

void turn_left(){
    set_direction_left();
    set_right_speed(3);
    vTaskDelay(950);
}
