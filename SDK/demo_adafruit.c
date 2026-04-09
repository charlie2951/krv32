/* Demo of sending sensor data to Adafruit IoT cloud
Using http POST Method
Author: Subir Kr. maity
*/
#include "esp_wifi.h"
#include"delay.h"
#include "printf.h"
#include "my_string.h"
#include "uart.h"

//Wifi Credentials and API keys
char * AP = "realmep3";       // Add your mobile hotspot name here
char * PASS = "12345687"; // Add password
char * HOST = "io.adafruit.com";
int PORT=80;
char * KEY = "aio_QSJp66LcMFWo1nCSQr4gw0Vdhi44";   // Add Adafruit X-AIO Key here
char * URL = "/api/v2/skmaityfet/feeds/temperature/data";  // Add URL of your feed here
//Buffers to store data
char atcommand[250] = {0,};
char data[250] = {0,};
char payload[250] = {0,};
//helper functions
void transmit_data(int sensor_data)
{
    ESP_SendCommand("AT+CIPMUX=0");  // This AT Command is used to enable or disable multiple TCP Connections. (0: Single connection, 1: Multiple connections)
    delay(5000);
    my_memset(atcommand,0,250);
    my_sprintf(atcommand,"AT+CIPSTART=\"TCP\",\"%s\",%d", HOST, PORT); // to establish one of the three connections: TCP, UDP or SSL. 
    ESP_SendCommand(atcommand);
    delay(10000);
    my_memset(atcommand,0,250);
    my_memset(data,0,250);
    my_sprintf(data,"{\"value\": %d}",sensor_data);
    my_sprintf(payload,"POST %s HTTP/1.1\r\nHost: %s\r\nContent-Type: application/json\r\nX-AIO-Key: %s\r\nContent-Length: %d\r\n\r\n%s",URL, HOST, KEY, my_strlen(data),data);
    my_sprintf(atcommand,"AT+CIPSEND=%d",my_strlen(payload)+2);  //  to start sending data in transparent transmission mode.
    ESP_SendCommand(atcommand);
    /*instead of custom library API, this raw low level API 
    is used to send the payload, because it will not add any additional
    \r\n unlike ESPSendCommand()
    */
    uart_sendline(UART2,payload);//send payload to ESP01 wifi module via uart2
    //ESP_SendCommand(payload);
    ESP_write_enter();
    //printing payload for debug
    printf("\r\nPayload is: %s", payload);
    printf("\r\nAT+CIPSEND=%d",my_strlen(payload));
    delay(10000000);
    ESP_SendCommand("AT+CIPCLOSE=0");  // Closes TCP/UDP/SSL connection 
}

//Main function
int main()
{
    // Initial ESP and WiFi setup...
    ESP_Init();
    delay(1000000);
    //Set to Station Mode
    ESP_SetMode(1);
    printf("Mode set to Station..\n");
    delay(2000000);
    ESP_ConnectToAP(AP,PASS);
    delay(10000000); //wait a while to connect with wifi

while(1){
    //place your code here
    int sensor_data = 25; //dummy sensor data
    printf("\r\nSending data to cloud...");
    transmit_data(sensor_data); //transmit data to cloud 
    delay(8000000);
}
    return 0;
}