#include "ultrasonic.h"

uint64_t width;
int successful_pulse;
absolute_time_t start_time;
absolute_time_t end_time;
uint final_result = -1;

void initUltrasonic(){

    gpio_init(UltrasonicEcho);
    gpio_init(UltrasonicTrig);
    
    gpio_set_dir(UltrasonicEcho, GPIO_IN);
    gpio_set_dir(UltrasonicTrig, GPIO_OUT);
}

void pulseUltrasonic(){
    gpio_put(UltrasonicTrig, 1);
    vTaskDelay(1);  // Add a brief delay for the ultrasonic pulse
    gpio_put(UltrasonicTrig, 0);
}

void shootUltrasonic(){
    while(1){
        vTaskDelay(10);
        printf("shooting\n");
        // Function to pulse ultrasonic sensor.
        pulseUltrasonic();
    }
}

uint64_t getDistanceUltrasonic() {

    width = 0;
    successful_pulse = 0;

    // while (gpio_get(ULTRASONIC_ECHO) == 0) {
    //     tight_loop_contents();
    // }

    start_time = get_absolute_time();

    while (gpio_get(UltrasonicEcho) == 1) {
        width++;

        if (width > ULTRASONIC_TIMEOUT) {
            successful_pulse = 0;
        }
        else {
            successful_pulse = 1;
        }
    }

    if (successful_pulse == 1) {
        end_time = get_absolute_time();
        final_result = absolute_time_diff_us(start_time, end_time) / 29 / 2; // Calculate distance based on time difference
        return final_result;
    } else {
        return -1;  // Return error value if measurement was unsuccessful
    }

}
