#include <stdio.h>
#include "pico/stdlib.h"

#include "hardware/gpio.h"
//#include "FreeRTOS.h"
//#include "task.h"
//#include "message_buffer.h"

#include "../../driver/motor/motor.h"

extern const uint WHEEL_EN_LEFT_OUT;
extern const uint WHEEL_EN_LEFT_VCC;
extern const uint WHEEL_EN_RIGHT_OUT;
extern const uint WHEEL_EN_RIGHT_VCC;

extern volatile int currentSpeedLeft;
extern volatile int currentSpeedRight;

//extern MessageBufferHandle_t rightSpeedBuffer;

void initWheelEncoderLeft();
void initWheelEncoderRight();
uint64_t measureSpeedLeft(int distance);
uint64_t measureSpeedRight();

void move(int distance, int direction);
void moveBack(int distance); 