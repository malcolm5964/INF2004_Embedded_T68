#include <stdio.h>
#include "pico/stdlib.h"
#include <stdlib.h>
#include <math.h>

#include "hardware/gpio.h"
#include "hardware/adc.h"
#include <time.h>


#define CENTER_IR_PIN_ADC 26

#define WHITE_SURFACE 0 // 0 signifies white
#define BLACK_SURFACE 1 // 1 signifies black

#define IR_SENSOR_SAMPLE_SIZE 50 // number of samples to collect
#define IR_SENSOR_DIFFERENCE 10 // difference before average sample is updated 
#define IR_SENSOR_THRESHOLD 2000 // threshold to determine black or white

extern volatile char print_character;

typedef struct barcode_character_t { // barcode character struct
    char character; // the character itself
    uint32_t black_value; // value of black
    uint32_t white_value; // value of white
} barcode_character_t;

typedef struct node_t { // node struct - for linked list
    int64_t duration; // duration of the bar
    struct node_t* next_node; // pointer to the next node
} node_t;

typedef struct bars_t { // bars struct - linked list
    struct node_t* start_node; // start node of linked list
    struct node_t* end_node; // end node of linked list
    uint8_t length; // length of linked list
} bars_t;

void barcode_inithello();