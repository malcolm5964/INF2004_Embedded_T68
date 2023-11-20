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

    if(direction == 0)
    {
        set_direction_forward();
    }
    if(direction == 1)
    {
        set_direction_back();
    }

    set_left_speed(13500);
    set_right_speed(16500);

    int leftTargetSpeed = 50;
    int rightTargetSpeed = 50;

    absolute_time_t leftPreviousTime = get_absolute_time();
    absolute_time_t rightPreviousTime = get_absolute_time();

    uint8_t leftBlackToWhite = 0;
    uint8_t rightBlackToWhite = 0;

    uint8_t leftBlackDetect;
    uint8_t rightBlackDetect;

    int leftDistanceMoved = 0;
    int rightDistanceMoved = 0;

    while(true)
    {
        printf("Wheel Econder raw: %i\n", gpio_get(WHEEL_EN_LEFT_OUT));
        //PWM for LEFT wheel
        //if((absolute_time_diff_us(leftPreviousTime, get_absolute_time()) > 1000000))
        //{
        //    currentSpeedLeft = 0;
        //    move_forward_left(leftTargetSpeed, currentSpeedLeft);
        //}

        //Detect change 
        if(gpio_get(WHEEL_EN_LEFT_OUT) == 1)
        {
            
            leftBlackDetect = 1;
        }
        if((gpio_get(WHEEL_EN_LEFT_OUT) == 0) && (leftBlackDetect == 1))
        {
            leftBlackDetect = 0;
            leftBlackToWhite += 1; 
        }
        //Calculate speed,distance and PWM
        if (leftBlackToWhite == 2)
        {
            leftBlackToWhite = 0;
            leftBlackDetect = 0;

            //Add distance (Circumference = 20.4cm)
            leftDistanceMoved += 1;
            if(leftDistanceMoved >= distance)
            {
                //printf("moved %icm\n", distance);
                leftTargetSpeed = 0;
            }

            absolute_time_t current_time = get_absolute_time();
            uint64_t time = absolute_time_diff_us(leftPreviousTime, current_time);

            //Speed (mm/s) = (Distance (10mm) * 1,000,000) / Time (microseconds)
            int speed = 10000000 / time;

            currentSpeedLeft = speed;
            printf("Wheel Speed left: %imm/s\n", speed);
            move_forward_left(leftTargetSpeed, currentSpeedLeft, direction);
            leftPreviousTime = current_time;
        }


        //PWM for RIGHT wheel
        //if((absolute_time_diff_us(rightPreviousTime, get_absolute_time()) > 1000000))
        //{
        //    currentSpeedRight = 0;
        //    move_forward_right(rightTargetSpeed, currentSpeedRight);
        //}
        if (gpio_get(WHEEL_EN_RIGHT_OUT) == 1)
        {
            rightBlackDetect = 1;
        }
        if((gpio_get(WHEEL_EN_RIGHT_OUT) == 0) && (rightBlackDetect == 1))
        {
            rightBlackDetect = 0;
            rightBlackToWhite += 1; 
        }
        if (rightBlackToWhite == 2)
        {
            rightBlackToWhite = 0;
            rightBlackDetect = 0;

            //Add distance (Circumference = 20.4cm)
            rightDistanceMoved += 1;
            if(rightDistanceMoved >= distance)
            {
                //printf("moved %icm\n", distance);
                rightTargetSpeed = 0;
            }

            absolute_time_t current_time = get_absolute_time();
            uint64_t time = absolute_time_diff_us(rightPreviousTime, current_time);

            //Speed (mm/s) = (Distance (10mm) * 1,000,000) / Time (microseconds)
            int speed = 10000000 / time;

            currentSpeedRight = speed;
            printf("Wheel Speed right: %imm/s\n", speed);
            move_forward_right(rightTargetSpeed, currentSpeedRight, direction);
            rightPreviousTime = current_time;
        }


        if(leftDistanceMoved >= distance && rightDistanceMoved >= distance)
        {
            printf("moved %icm\n", distance);
            break;
        }
    }
}

//uint64_t measureSpeedRight(int distance){
//    
//    absolute_time_t previousTime = get_absolute_time();
//    uint8_t detect_change;
//    int distanceMoved = 0;
//
//    while (true)
//    {
//        if((absolute_time_diff_us(previousTime, get_absolute_time()) > 1000000))
//        {
//            currentSpeedRight = 0;
//            printf("Wheel Speed Right: %imm/s\n", currentSpeedRight);
//            move_forward_right(120, currentSpeedLeft);
//        }
//        if (gpio_get(WHEEL_EN_RIGHT_OUT) == 1)
//        {
//            detect_change = 1;
//        }
//        if ((gpio_get(WHEEL_EN_RIGHT_OUT) == 0) && (detect_change == 1))
//        {
//            detect_change = 0;
//
//            //Add distance (Circumference = 20.4cm)
//            distanceMoved += 1;
//            if(distanceMoved > distance)
//            {
//                printf("moved %icm\n", distance);
//                break;
//            }
//
//            absolute_time_t current_time = get_absolute_time();
//            int64_t time = absolute_time_diff_us(previousTime, current_time);
//
//            //Speed (mm/s) = (Distance (10mm) * 1,000,000) / Time (microseconds)
//            int speed = 10000000 / time;
//            if(speed > 1000)
//            {
//                printf("test\n");
//                currentSpeedRight = 0;
//            }
//            else
//            {
//                currentSpeedRight = speed;
//                move_forward_right(120, currentSpeedLeft);
//                printf("Wheel Speed Right: %imm/s\n", speed);
//                previousTime = current_time;
//            }
//        }
//    }
//}
//
//
//uint64_t measureSpeedLeft(int distance){
//    absolute_time_t previousTime = get_absolute_time();
//    uint8_t detect_change;
//    int distanceMoved = 0;
//
//    while (true)
//    {   
//        //Wheel is not moving
//        if((absolute_time_diff_us(previousTime, get_absolute_time()) > 1000000))
//        {
//            currentSpeedLeft = 0;
//            move_forward_left(120, currentSpeedLeft);
//        }
//        if (gpio_get(WHEEL_EN_LEFT_OUT) == 1)
//        {
//            detect_change = 1;
//        }
//        if ((gpio_get(WHEEL_EN_LEFT_OUT) == 0) && (detect_change == 1))
//        {
//            detect_change = 0;
//
//            //Add distance (Circumference = 20.4cm)
//            distanceMoved += 1;
//            if(distanceMoved > distance)
//            {
//                printf("moved 10cm\n");
//                break;
//            }
//
//            absolute_time_t current_time = get_absolute_time();
//            uint64_t time = absolute_time_diff_us(previousTime, current_time);
//
//            //Speed (mm/s) = (Distance (10mm) * 1,000,000) / Time (microseconds)
//            int speed = 10000000 / time;
//            if(speed > 1000)
//            {
//                printf("test\n");
//                currentSpeedRight = 0;
//            }
//            else
//            {
//            currentSpeedLeft = speed;
//            move_forward_left(120, currentSpeedLeft);
//            printf("Wheel Speed left: %imm/s\n", speed);
//            previousTime = current_time;
//            }
//        }
//    }
//}