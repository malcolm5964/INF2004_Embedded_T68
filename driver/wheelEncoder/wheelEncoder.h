#include <stdio.h>
#include "pico/stdlib.h"

#include "hardware/gpio.h"


extern const uint WHEEL_EN_LEFT_OUT;
extern const uint WHEEL_EN_LEFT_VCC;
extern const uint WHEEL_EN_RIGHT_OUT;
extern const uint WHEEL_EN_RIGHT_VCC;
extern volatile int currentSpeedLeft;
extern volatile int currentSpeedRight;

void initWheelEncoderLeft();
void initWheelEncoderRight();
uint64_t measureSpeedLeft();
uint64_t measureSpeedRight();