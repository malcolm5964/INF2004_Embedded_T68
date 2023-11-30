#include <stdio.h>
#include "pico/stdlib.h"
#include <math.h>
#include "hardware/gpio.h"

//WIFI
#include "pico/cyw43_arch.h"
#include "lwip/apps/httpd.h"
#include "lwipopts.h"
#include "ssi.h"
#include "cgi.h"


//FreeROTS 
#include "FreeRTOS.h"
#include "task.h"
#include "message_buffer.h"

//Drivers
#include "driver/motor/motor.h"
#include "driver/magnometer/magnometer.h"
#include "driver/wheelEncoder/wheelEncoder.h"
#include "driver/irline/irline.h"
//#include "driver/mapping/mapping.h"
#include "driver/ultrasonic/ultrasonic.h"
//#include "driver/barcode/barcode.h"


// WIFI Credentials - take care if pushing to github!
const char WIFI_SSID[] = "xxx";
const char WIFI_PASSWORD[] = "xxx";


void mapping_task(__unused void *params)
{
    vTaskDelay(1000);
    initWheelEncoderLeft();
    initWheelEncoderRight();
    init_right_motor();
    init_left_motor();
    printf("Wheel Init\n");
    vTaskDelay(10000);
    initGraph();
    while(1);
}


void irline_task(__unused void *params)
{
    vTaskDelay(1000);
    init_ir();
    get_ir_value();
}


void magnometer_task(__unused void *params)
{
    vTaskDelay(2000);
    magnometer_init();
    //calibrate_turn();
    //magnometer_calibrate();
    //stop();
    magnometer_read();
}


void webServer_task(__unused void *params){
    cyw43_arch_init();

    cyw43_arch_enable_sta_mode();

    // Connect to the WiFI network - loop until connected
    while(cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 15000) != 0){
        printf("Attempting to connect...\n");
    }
    // Print a success message once connected
    printf("Connected! \n");
    
    // Initialise web server
    httpd_init();
    printf("Http server initialised\n");

    //Configure SSI and CGI handler
    ssi_init(); 
    printf("SSI Handler initialised\n");
    cgi_init();
    printf("CGI Handler initialised\n");

    while(1);
}



void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == ULTRASONIC_ECHO)
    {
        getDistanceUltrasonic(NULL);
    }
}

void interrupt_task(__unused void *params) {

    // vTaskDelay(5000);
    //PICO_MAX_SHARED_IRQ_HANDLERS;
    gpio_set_irq_enabled_with_callback(ULTRASONIC_ECHO, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);

    while (true)
    {
        tight_loop_contents();
    }
}

void shootUltrasonic_task(__unused void *params){

    vTaskDelay(5000);
    printf("hi");
    initUltrasonic(NULL);
    while(true){
        pulseUltrasonic(NULL);
        // if (final_result !=-1)
        // {
        // printf("Distance: %d cm\n", final_result);
        // final_result = -1;
        // }
        if (final_result < 10)
        {
            stop();
            printf("Stop");
            moveBack(10);
            final_result = -1;
        }
    }
}

void barcode_task(__unused void *params) {
    stdio_init_all();
    vTaskDelay(4000);

    barcode_inithello();

    while(1);
}




void vLaunch(void)
{
    TaskHandle_t webServer;
    xTaskCreate(webServer_task, "WebServer", configMINIMAL_STACK_SIZE, NULL, 2, &webServer);

    TaskHandle_t mapping;
    xTaskCreate(mapping_task, "MappingMap", configMINIMAL_STACK_SIZE, NULL, 2, &mapping);
    
    //TaskHandle_t irline;
    //xTaskCreate(irline_task, "Irline", configMINIMAL_STACK_SIZE, NULL, 2, &irline);

    //TaskHandle_t magnometer;
    //xTaskCreate(magnometer_task, "Magnometer", configMINIMAL_STACK_SIZE, NULL, 2, &magnometer);

    //TaskHandle_t shootUltrasonicSensorTask;
    //xTaskCreate(shootUltrasonic_task, "ShootUltrasonicSensorThread", configMINIMAL_STACK_SIZE, NULL, 2, &shootUltrasonicSensorTask);
//
    //TaskHandle_t interruptTask;
    //xTaskCreate(interrupt_task, "InterruptThread", configMINIMAL_STACK_SIZE, NULL, 2, &interruptTask);

    //TaskHandle_t barcodeTask;
    //xTaskCreate(barcode_task, "barcode", configMINIMAL_STACK_SIZE, NULL, 2, &barcodeTask);

    vTaskStartScheduler();
}




int main() {
    stdio_init_all();
    stdio_usb_init();

    vLaunch();

    return 0;
}