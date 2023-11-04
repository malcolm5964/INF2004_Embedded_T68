#include <stdio.h>
#include "pico/stdlib.h"

//WIFI
#include "pico/cyw43_arch.h"
#include "lwip/apps/httpd.h"
#include "lwipopts.h"
#include "ssi.h"
#include "cgi.h"
#include "lwip/netif.h"
#include "lwip/ip4_addr.h"

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
const char WIFI_SSID[] = "ASUS_C8_5G";
const char WIFI_PASSWORD[] = "Malcolm2069";



void moving_task(__unused void *params)
{
    vTaskDelay(1000);
    init_right_motor();
    init_left_motor();

    while (true)
    {   //Test Forward
        move_forward();
        vTaskDelay(5000);
        //Test Right
        //turn_right();
        ////Test Backwards
        //move_backward();
        //vTaskDelay(5000);
        //stop();
    }
    
}

void vLaunch(void)
{
    TaskHandle_t moveCar;
    xTaskCreate(moving_task, "MoveCar", configMINIMAL_STACK_SIZE, NULL, 3, &moveCar);

    vTaskStartScheduler();
}

void connectWifi(){
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

    // Configure SSI and CGI handler
    ssi_init(); 
    printf("SSI Handler initialised\n");
    cgi_init();
    printf("CGI Handler initialised\n");
}


int main() {
    stdio_init_all();
    stdio_usb_init();

    connectWifi();

    vLaunch();

    return 0;
}