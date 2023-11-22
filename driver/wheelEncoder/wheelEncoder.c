#include "wheelEncoder.h"

//Wheel IR Sensor
const uint WHEEL_EN_LEFT_OUT = 2;

const uint WHEEL_EN_RIGHT_OUT = 0;
const uint WHEEL_EN_RIGHT_GND = 1;

//#define mbaTASK_MESSAGE_BUFFER_SIZE (60)
//MessageBufferHandle_t rightSpeedBuffer;

void initWheelEncoderLeft(){
    printf("Wheel encoder left init\n");
    // Init Left IR Sensor out
    gpio_init(WHEEL_EN_LEFT_OUT);
    gpio_set_dir(WHEEL_EN_LEFT_OUT, GPIO_IN);
}

void initWheelEncoderRight(){
    //rightSpeedBuffer = xMessageBufferCreate(mbaTASK_MESSAGE_BUFFER_SIZE);

    printf("Wheel encoder Right init\n");
    // Init Right IR Sensor
    gpio_init(WHEEL_EN_RIGHT_OUT);
    gpio_set_dir(WHEEL_EN_RIGHT_OUT, GPIO_IN);
    gpio_init(WHEEL_EN_RIGHT_GND);
    gpio_set_dir(WHEEL_EN_RIGHT_GND, GPIO_OUT);
    gpio_put(WHEEL_EN_RIGHT_GND, 0); // Set to GND
}

volatile int currentSpeedLeft;
volatile int currentSpeedRight;

void move(int distance, int direction){

    if(direction == 0){
        set_direction_forward();
    }
    if(direction == 1){
        set_direction_back();
    }

    double startHeading = headingPass;
    double lowHeading = headingPass - 2.5;
    double highHeading = headingPass + 2.5;

    printf("Range : %f to %f\n", lowHeading, highHeading);

    set_left_speed(18500);
    set_right_speed(18500);

    int leftDetect_change;
    int rightDetect_change;

    absolute_time_t leftPreviousTime = get_absolute_time();
    absolute_time_t rightPreviousTime = get_absolute_time();

    int leftDistanceMoved = 0;
    int rightDistanceMoved = 0;

    while(true)
    {
        //printf("PWM: %i\n", PWM_right);
        printf("Current headint %f\n", headingPass);
        //Magnometer HELP
        if(!((headingPass > lowHeading) && (headingPass < highHeading)))
        {
            if(headingPass < lowHeading)
            {
                PWM_left += 3;
                set_left_speed(PWM_left);
                printf("Increasing left\n");
            }
            if(headingPass > highHeading)
            {
                PWM_right += 3;
                set_right_speed(PWM_right);
                printf("Increasing right\n");
            }
        }
        
        //BLACK LINE HELP
        //0 = white
        //if(left_ir() == 0)
        //{
        //    PWM_right = PWM_right + 5;
        //    printf("LEFT on white %i\n", PWM_right);
        //    set_right_speed(PWM_right);
        //}
//
        //if(right_ir() == 0)
        //{
        //    PWM_right = PWM_right - 5;
        //    printf("RIGHT on White %i\n", PWM_right);
        //    set_right_speed(PWM_right);
        //}
        


        //Left Wheel
        if(gpio_get(WHEEL_EN_LEFT_OUT) == 1)
        {

            leftDetect_change = 1;
        }
        //Calculate speed,distance and PWM
        if ((gpio_get(WHEEL_EN_LEFT_OUT) == 0) && (leftDetect_change == 1))
        {
            leftDetect_change = 0;

            //Add distance (Circumference = 20.4cm)
            leftDistanceMoved += 1;
            if(leftDistanceMoved >= distance)
            {
                //printf("moved %icm\n", distance);
                set_left_speed(0);
            }

            absolute_time_t current_time = get_absolute_time();
            uint64_t time = absolute_time_diff_us(leftPreviousTime, current_time);

            //Speed (mm/s) = (Distance (10mm) * 1,000,000) / Time (microseconds)
            int speed = 10000000 / time;

            currentSpeedLeft = speed;
            printf("Wheel Speed left: %imm/s\n", speed);
            if(leftDistanceMoved > 1 && leftDistanceMoved < distance)
            {
                //left_PID(leftTargetSpeed, currentSpeedLeft, direction);
            }
            leftPreviousTime = current_time;
        }

        //Right Wheel
        if (gpio_get(WHEEL_EN_RIGHT_OUT) == 1)
        {
            rightDetect_change = 1;
        }
        if ((gpio_get(WHEEL_EN_RIGHT_OUT) == 0) && (rightDetect_change == 1))
        {
            rightDetect_change = 0;

            //Add distance (Circumference = 20.4cm)
            rightDistanceMoved += 1;
            if(rightDistanceMoved >= distance)
            {
                //printf("moved %icm\n", distance);
                set_right_speed(0);
            }

            absolute_time_t current_time = get_absolute_time();
            uint64_t time = absolute_time_diff_us(rightPreviousTime, current_time);

            //Speed (mm/s) = (Distance (10mm) * 1,000,000) / Time (microseconds)
            int speed = 10000000 / time;

            currentSpeedRight = speed;
            printf("Wheel Speed right: %imm/s\n", speed);
            if(rightDistanceMoved > 1 && rightDistanceMoved < distance)
            {
                //right_PID(currentSpeedLeft, currentSpeedRight, direction);
            }
            rightPreviousTime = current_time;
        }


        if(leftDistanceMoved >= distance && rightDistanceMoved >= distance)
        {
            printf("moved %icm\n", distance); 
            PWM_left = 18500;
            PWM_right = 18500;
            break;
        }
    }
}

