/*
Sample test program from scratch to send data to adafruit cloud
*/

#include <stdint.h>
#include "uart.h"
#include "delay.h"
#include "my_string.h"


#define ESP_UART UART2
#define SYS_UART UART1

char* API_KEY = "BE1M4NDJRKO5W1O8" ;  // Replace Key by your own

//ESP wifi module helper functions
int write_to_esp8266(char *data)
{
   	while (*data != '\0')
	{
		uart_send(ESP_UART, *data);
		data++;
	}
	uart_sendline(ESP_UART, "\r\n");
	
}

int write_enter_to_esp8266()
{
	uart_sendline(ESP_UART,"\r\n");
    
}

//Setting up wifi module
void setup_esp8266()
{
	char data[200];
	write_to_esp8266("AT");
	delay(2000000);
	uart_sendline(SYS_UART," set esp8266 as AP\n");
	write_to_esp8266("AT+CWMODE=1");
	delay(1000000);
	uart_sendline(SYS_UART," Connect esp8266 to AP \n");
	write_to_esp8266("AT+CWJAP=\"Airtel_subir_airtel\",\"kiit2951\"");
	delay(1000000);
	uart_sendline(SYS_UART,"sending AT Echo off command to esp\n");
	write_to_esp8266("ATE0");
	delay(5000);
	uart_sendline(SYS_UART," write AT+CIPMUX\n");
	write_to_esp8266("AT+CIPMUX=0");
	delay(5000);
}

// Transmit data packet over TCP in thingspeak cloud
void transmit_data(int temperature, int pressure)
{
    char data[200];
	char sendData[200];
	char CIPSEND[14];
	int length;
    char len_val[10];
	//clear buffer
	my_memset(sendData,0,200);
	//sprintf(sendData, "GET https://api.thingspeak.com/update?api_key=%s&field1=%u.%u&field2=%u.%u", API_KEY, temperature/10, temperature%10, pressure / 100, pressure % 100);
    //sprintf(sendData, "GET https://api.thingspeak.com/update?api_key=%s&field1=%u.%u", API_KEY, temperature/10, temperature%10);
	//Building payload
    my_strcpy(sendData, "GET https://api.thingspeak.com/update?api_key=");
    my_strcat(sendData, API_KEY);
    my_strcat(sendData,"&field1=");//dummy data 25.43 in payload to test connection
    my_memset(len_val,0,10);//clear buffer
    my_int2str(temperature,len_val);//convert temp to string
    my_strcat(sendData,len_val);
    //length = strlen(sendData);
    length = my_strlen(sendData);
	length = length + 2;//for last '/r/n'
    my_memset(CIPSEND,0,14);
    my_memset(len_val,0,10);//clear buffer
	my_strcpy(CIPSEND, "AT+CIPSEND=");
    my_int2str(length,len_val);//convert total length to string
    my_strcat(CIPSEND,len_val);
    //debug
    uart_sendline(SYS_UART,"Total payload length: ");
    uart_sendline(SYS_UART,len_val);
	// Connect to Wifi using esp8266
	uart_sendline(SYS_UART,"\n Open connection to thingspeak.com\n");
	uart_sendline(SYS_UART,"\n sending data..\r");
	write_to_esp8266("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80");
	delay(1000);
	write_to_esp8266(CIPSEND);
	delay(100000);
	uart_sendline(SYS_UART," write Data\n");
	write_to_esp8266(sendData);
	write_enter_to_esp8266();
	// disconnect from link
	uart_sendline(SYS_UART," write AT+CIPCLOSE\n");
	write_to_esp8266("AT+CIPCLOSE");
	
}

int main()
{ 
    uint8_t i=0;
    char data[200];
    //uint32_t temperature;
    int sensor2_val = 0;
    //initialize wifi connection
    uart_sendline(SYS_UART,"\n Waiting to ESP8266 to initialize... \n\r");
	delay(2000000);
	setup_esp8266();
   while(1){
    if (i>10)
	{
		i=0;
	}
	   
    transmit_data(i*2,sensor2_val);//send 2 sensor data to adafruit cloud
    delay(10000000);
    i++;
   }
   return 0;
}