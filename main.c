#include <stdio.h>
#include "pico/stdlib.h"
#include <math.h>

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
#include "driver/mapping/mapping.h"
#include "driver/ultrasonic/ultrasonic.h"


// WIFI Credentials - take care if pushing to github!
const char WIFI_SSID[] = "koh_family";
const char WIFI_PASSWORD[] = "koh103325";


void mapping_task(__unused void *params)
{
    vTaskDelay(5000);
    initWheelEncoderLeft();
    initWheelEncoderRight();
    init_right_motor();
    init_left_motor();
    vTaskDelay(3000);
    //turn_right();
    move(10, 0);
    move(19, 0);
    move(19, 0);
    //move(19, 0);
    //turn_right();
    //initGraph();
}


void irline_task(__unused void *params)
{
    vTaskDelay(5000);
    init_ir();
    get_ir_value();
}

void magnometer_task(__unused void *params)
{
    vTaskDelay(5000);
    magnometer_init();
    magnometer_read();
}


void webServer_task(__unused void *params){
    cyw43_arch_init();

    cyw43_arch_enable_sta_mode();

    // Connect to the WiFI network - loop until connected
    while(cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000) != 0){
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
    if (gpio == UltrasonicEcho) {
        getDistanceUltrasonic();
    }
}

void interrupt_task(__unused void *params) {

    gpio_set_irq_enabled_with_callback(UltrasonicEcho, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);

    while (true) {
        tight_loop_contents();
    }
}

void shootUltrasonic_task(__unused void *params){

    initUltrasonic();
    shootUltrasonic();

}




void vLaunch(void)
{
    TaskHandle_t webServer;
    xTaskCreate(webServer_task, "WebServer", configMINIMAL_STACK_SIZE, NULL, 3, &webServer);

    //TaskHandle_t mapping;
    //xTaskCreate(mapping_task, "MappingMap", configMINIMAL_STACK_SIZE, NULL, 3, &mapping);
    
    TaskHandle_t irline;
    xTaskCreate(irline_task, "Irline", configMINIMAL_STACK_SIZE, NULL, 3, &irline);

    TaskHandle_t magnometer;
    xTaskCreate(magnometer_task, "Magnometer", configMINIMAL_STACK_SIZE, NULL, 3, &magnometer);

    TaskHandle_t shootUltrasonicSensorTask;
    xTaskCreate(shootUltrasonic_task, "ShootUltrasonicSensorThread", configMINIMAL_STACK_SIZE, NULL, 3, &shootUltrasonicSensorTask);

    TaskHandle_t interruptTask;
    xTaskCreate(interrupt_task, "InterruptThread", configMINIMAL_STACK_SIZE, NULL, 3, &interruptTask);

    vTaskStartScheduler();
}




int main() {
    stdio_init_all();
    stdio_usb_init();

    vLaunch();

    return 0;
}