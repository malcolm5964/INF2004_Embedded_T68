#include <stdio.h>
#include "pico/stdlib.h"

#include "hardware/gpio.h"

#define UltrasonicEcho 3
#define UltrasonicTrig 4
#define ULTRASONIC_TIMEOUT 26100

void initUltrasonic();
void pulseUltrasonic();
void shootUltrasonic();

uint64_t getDistanceUltrasonic();