#ifndef _ULTRASONIC_H
#define _ULTRASONIC_H

#include "ultrasonic.h"
#include "hardware/gpio.h"

// Definitions for ultrasonic sensor pins and timeout value
#define ULTRASONIC_ECHO 4
#define ULTRASONIC_TRIG 5
#define ULTRASONIC_TIMEOUT 26100

// Global variable declaration for storing the final ultrasonic distance result
extern uint final_result;

// Function declarations for ultrasonic sensor operations
void initUltrasonic(void *params);
void pulseUltrasonic(void *params);
uint64_t getDistanceUltrasonic(void *params);
void gpio_callback_ultrasonic(uint gpio, uint32_t events);

#endif

/*** End of file ***/