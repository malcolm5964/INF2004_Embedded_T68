#include "barcode.h"

volatile uint8_t current_detected_surface = WHITE_SURFACE;
volatile uint8_t previous_detected_surface = WHITE_SURFACE;

volatile uint8_t black_value, white_value, status;
volatile uint16_t current_sample_average, previous_sample_average;
volatile uint32_t sample_index, sample_sum;

volatile int64_t duration_sum, duration_average;

node_t node = { 0, NULL };

volatile char print_character = 0;

absolute_time_t barcodeStart_time;
absolute_time_t barcodeEnd_time;

int counter = 0;

/* START OF BARCODE CHARACTERS */
barcode_character_t barcode_characters[27] = 
{
    { 'A', 0x11, 0x02 },
    { 'B', 0x09, 0x02 }, 
    { 'C', 0x18, 0x02 }, 
    { 'D', 0x05, 0x02 },
    { 'E', 0x14, 0x02 }, 
    { 'F', 0x0C, 0x02 },
    { 'G', 0x03, 0x02 }, 
    { 'H', 0x12, 0x02 }, 
    { 'I', 0x0A, 0x02 }, 
    { 'J', 0x06, 0x02 }, 
    { 'K', 0x11, 0x01 }, 
    { 'L', 0x09, 0x01 }, 
    { 'M', 0x18, 0x01 }, 
    { 'N', 0x05, 0x01 }, 
    { 'O', 0x14, 0x01 }, 
    { 'P', 0x0C, 0x01 },  
    { 'Q', 0x03, 0x01 }, 
    { 'R', 0x12, 0x01 }, 
    { 'S', 0x0A, 0x01 },
    { 'T', 0x06, 0x01 }, 
    { 'U', 0x11, 0x08 }, 
    { 'V', 0x09, 0x08 }, 
    { 'W', 0x18, 0x08 },
    { 'X', 0x05, 0x08 }, 
    { 'Y', 0x14, 0x08 }, 
    { 'Z', 0x0C, 0x08 },
    { '*', 0x06, 0x08 },
};

barcode_character_t barcode_characters_reverse[27] = 
{
    { 'A', 0x11, 0x02 },
    { 'B', 0x09, 0x02 }, 
    { 'C', 0x18, 0x02 }, 
    { 'D', 0x05, 0x02 },
    { 'E', 0x14, 0x02 }, 
    { 'F', 0x0C, 0x02 },
    { 'G', 0x03, 0x02 }, 
    { 'H', 0x12, 0x02 }, 
    { 'I', 0x0A, 0x02 }, 
    { 'J', 0x06, 0x02 }, 
    { 'K', 0x11, 0x01 }, 
    { 'L', 0x09, 0x01 }, 
    { 'M', 0x18, 0x01 }, 
    { 'A', 0x14, 0x01 }, 
    { '*', 0x0C, 0x01 },  
    { 'R', 0x12, 0x01 }, 
    { 'S', 0x0A, 0x01 },
    { 'A', 0x06, 0x01 }, 
    { 'U', 0x11, 0x08 }, 
    { 'V', 0x09, 0x08 }, 
    { 'W', 0x03, 0x01 },
    { 'X', 0x14, 0x01 }, 
    { 'Y', 0x05, 0x01 }, 
    { 'Z', 0x06, 0x01 },
    { '*', 0x06, 0x08 },
};

char barcode_characters_find(uint8_t black_value, uint8_t white_value, barcode_character_t *barcode_characters) { // find a barcode character
    for (uint8_t i = 0; i < 27; i++) { // iterate through all 27 characters
        if (barcode_characters[i].black_value == black_value && barcode_characters[i].white_value == white_value) // compare the black and white values
            return barcode_characters[i].character; // return the character if match
    }
    return '\0'; // return terminating character if nothing found
}

bars_t bars = { NULL, NULL, 0 }; // linked list

void bars_add(int64_t duration, bars_t *bars) { // add a bar
    //printf("add bar\n");
    node_t *node = malloc(sizeof(node_t)); // allocate memory for a new node
    node->duration = duration; // set the duration of the node
    node->next_node = NULL; // set the pointer of the next node

    if (bars->start_node == NULL) { // linked list is empty
        bars->start_node = bars->end_node = node; // set the start node & end node to the new node
        bars->length = 1; // set the length to 1
    } else if (bars->length == 9) { // linked list is full - 9 nodes
        node_t *delete_node = bars->start_node; // set the start node pointer for deletion

        bars->start_node = bars->start_node->next_node; // set the start node to the 2nd node
        bars->end_node->next_node = node; // set the 2nd last node's next node to the new node
        bars->end_node = node; // set the end node to the new node

        free(delete_node); // free the previous start node pointer to delete
    } else if (bars->length < 9) { // linked list is not empty & not full
        bars->end_node->next_node = node; // set the current end node's next node to the new node
        bars->end_node = node; // set the end node to the new node
        bars->length++; // increment the length of the linked list
    }
}

uint8_t bars_is_ready(bars_t *bars) { // is bar ready
    return bars->start_node != NULL && bars->end_node != NULL && bars->length == 9; // return true if start and end node is not empty, and has length of 9
}


void adc_handler(){

    if(counter == 0){
        barcodeStart_time = get_absolute_time(); // save the current time
    }
    
    
    if(!adc_fifo_is_empty()) {
        if(sample_index < IR_SENSOR_SAMPLE_SIZE) {
            sample_sum += adc_fifo_get();
            sample_index++;
        } else {
            current_sample_average = sample_sum / IR_SENSOR_SAMPLE_SIZE;

            if(abs(current_sample_average - previous_sample_average) >= IR_SENSOR_DIFFERENCE) {
                previous_sample_average = current_sample_average;

                previous_detected_surface = current_detected_surface;
                current_detected_surface = current_sample_average >= IR_SENSOR_THRESHOLD ? BLACK_SURFACE : WHITE_SURFACE;
                

                if(current_detected_surface != previous_detected_surface) {
                    //printf("*%c*\n", 66);
                    if(counter > 0){

                            printf("Bar start time: %llu, Bar end time: %llu\n", barcodeStart_time, get_absolute_time());
                            bars_add(absolute_time_diff_us(barcodeStart_time, get_absolute_time()), &bars);
                            barcodeStart_time = get_absolute_time();

                            if(bars_is_ready(&bars)) {
                                //printf("*%c*\n", 65);
                                node_t node = *bars.start_node;

                                for (uint8_t i = 0; i < 9; i++) {
                                    duration_sum += node.duration;
                                    node = *node.next_node;
                                }

                                duration_average = duration_sum / 9;
                                //printf("duration average: %i\n", duration_average);


                                node = *bars.start_node;
                                for(uint8_t i = 0; i < 9; i++) {
                                    //printf("black time: %i\n", node.duration);
                                    if (i % 2 == 0) {
                                        black_value = black_value << 1;
                                        if(node.duration >= duration_average)
                                            black_value = black_value | 0x1;
                                    } else {
                                        white_value = white_value << 1;
                                        if(node.duration >= duration_average)
                                            white_value = white_value | 0x1;
                                    }

                                    node = *node.next_node;
                                }

                                printf("Black Value: %i\n", black_value);
                                printf("White Value: %i\n", white_value);

                                char character = barcode_characters_find(black_value, white_value, barcode_characters_reverse);

                                if (character != '\0') { // valid character found
                                
                                    printf("*%c*\n", character);
                                    if(character != '*')
                                    {
                                        print_character = character;
                                    }
                                    
                                }
                                bars = (bars_t) { NULL, NULL, 0 }; // flush linked list
                                duration_sum = duration_average = black_value = white_value = 0;
                                counter = -1;
                            }   
                    }
                    counter++;

                }
            }

            sample_sum = 0; // reset the total sample sum
            sample_index = 0;
        }
    }

    irq_clear(ADC_IRQ_FIFO); // clear IRQ flag
}

void barcode_inithello(){
    printf("BARCODE READER IS READY!\n");

    adc_init();
    adc_gpio_init(CENTER_IR_PIN_ADC);
    adc_select_input(0);

    adc_fifo_setup(true, false, 1, false, false); // set up FIFO with 4 samples
    adc_irq_set_enabled(true); // enable interrupts for ADC
    adc_run(true); // enable free running mode
    
    irq_set_exclusive_handler(ADC_IRQ_FIFO, adc_handler); // set exclusive handler for ADC
	irq_set_enabled(ADC_IRQ_FIFO, true); // enable irq for ADC

    printf("BARCODE READER IS READY!\n");
}
