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

// WIFI Credentials - take care if pushing to github!
const char WIFI_SSID[] = "koh_family";
const char WIFI_PASSWORD[] = "koh103325";

#define mbaTASK_MESSAGE_BUFFER_SIZE (60)
static MessageBufferHandle_t leftSpeedBuffer;



void moving_task(__unused void *params)
{
    int64_t leftSpeed;
    vTaskDelay(10000);
    init_right_motor();
    init_left_motor();

    while (true)
    {   
        vTaskDelay(100);
        //Test Forward
        //move_forward_left(120, currentSpeedLeft); //(80mm)
        move_forward_right(120, currentSpeedRight);
        
        //vTaskDelay(5000);
    }
}

//void mapping_task(__unused void *params)
//{
//    initGraph(20); //20 vertex
//    connectEdge();
//}

void measureSpeed_task(__unused void *params)
{
    vTaskDelay(10000);
    //initWheelEncoderLeft();
    initWheelEncoderRight();
    //measureSpeedLeft();
    measureSpeedRight();
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



void vLaunch(void)
{
    TaskHandle_t moveCar;
    xTaskCreate(moving_task, "MoveCar", configMINIMAL_STACK_SIZE, NULL, 3, &moveCar);

    TaskHandle_t measureSpeed;
    xTaskCreate(measureSpeed_task, "MeasureSpeed", configMINIMAL_STACK_SIZE, NULL, 3, &measureSpeed);

    TaskHandle_t webServer;
    xTaskCreate(webServer_task, "WebServer", configMINIMAL_STACK_SIZE, NULL, 3, &webServer);

    //TaskHandle_t mapping;
    //xTaskCreate(mapping_task, "MappingMap", configMINIMAL_STACK_SIZE, NULL, 3, &mapping);

    leftSpeedBuffer = xMessageBufferCreate(mbaTASK_MESSAGE_BUFFER_SIZE);

    vTaskStartScheduler();
}




int main() {
    stdio_init_all();
    stdio_usb_init();

    vLaunch();

    return 0;
}