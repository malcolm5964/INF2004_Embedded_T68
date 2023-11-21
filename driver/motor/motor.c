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
    gpio_put(N4, 1);
}

void set_direction_left()
{
    gpio_put(N1, 1);
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
double kp = 2.5;

int16_t PWM_left = 14000;
int16_t PWM_right = 18000;


void left_PID(int target_speed, int current_speed, int direction){

    //printf("LEFT Target Speed: %i Current Speed: %i\n", target_speed, current_speed);

    if(target_speed == 0)
    {
        set_left_speed(0);
        PWM_left = 14000;
    }
    else
    {
        //Calculate error
        double error = target_speed - current_speed;

        //Calculate the Control Variable
        double proportional = kp * error;

        int16_t PWM = (int16_t)(proportional);

        PWM_left += PWM;

        set_left_speed(PWM_left);
        printf("LEFT PWM: %i\n", PWM_left);
    }
        
}

void right_PID(int target_speed, int current_speed, int direction){

    //printf("RIGHT Target Speed: %i Current Speed: %i\n", target_speed, current_speed);

    if(target_speed == 0)
    {
        set_right_speed(0);
        PWM_right = 18000;
    }
    else
    {
        //Calculate error
        double error = target_speed - current_speed;


        //Calculate the Control Variable
        double proportional = kp * error;

        int16_t PWM = (int16_t)(proportional);

        PWM_right += PWM;

        set_right_speed(PWM_right);
        printf("RIGHT PWM: %i\n", PWM_right);
    }
}

void move_backward(){
    set_direction_back();
    set_left_speed(3);
    set_left_speed(3);
}

void turn_right(){

    float startHeading = headingPass;

    printf("start heading: %f\n", headingPass);

    float endingHeading  = startHeading + 90;



    if(endingHeading > 360)
    {
        endingHeading = endingHeading - 360;
    }
    if(endingHeading < 0)
    {
        endingHeading = endingHeading + 360;
    }

    float endingHeadingLow = endingHeading - 1;
    float endingHeadingHigh = endingHeading + 1
    ;

    printf("start heading: %f, end heading: %f\n", endingHeadingLow, endingHeadingHigh);

    vTaskDelay(2000);
    set_direction_right();
    set_left_speed(15000);
    set_right_speed(15000);

    while (true)
    {
        if((headingPass > endingHeadingLow) && (headingPass < endingHeadingHigh))
        {
            printf("Stop: %i\n", headingPass);
            set_right_speed(0);
            set_left_speed(0);
        }
    }
    //vTaskDelay(1400);
    //if(right_ir() == 1)
    //{
    //    set_left_speed(0);
    //}
}

void turn_left(){
    set_direction_left();
    set_right_speed(16500);



    if(left_ir() == 1)
    {
        set_left_speed(0);
    }
}
