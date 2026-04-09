#include <stdint.h>
#include "uart.h"
#include "delay.h"
#include "my_string.h"

#define ESP_UART UART2

char * API_KEY="aio_QSJp66LcMFWo1nCSQr4gw0Vdhi44";   // Replace Key here
//char * URL="/api/v2/skmaityfet/feeds/temperature/data";  // Add URL of your feed here

int read_from_esp8266(char *data)
{
    uint8_t ch;
    char *ptr = data;
    int timeout = 2000000;

    my_memset(data, 0, 198);

    while (timeout--)
    {
        if (ESP_UART->STATUS != 0)//if ESP UART is available
        {
            ch = uart_receive(ESP_UART);

            *ptr++ = (char)ch;
            *ptr = '\0';

            uart_send(UART1, ch); // DEBUG

            if (my_strstr(data, "OK")) return 0;
            if (my_strstr(data, "ERROR")) return 1;
            if (my_strstr(data, ">")) return 3;
            if (my_strstr(data, "SEND OK")) return 4;
        }
    }

    return 2; // timeout
}

void print_char(char *data)
{
	while (*data != '\0')
	{
		uart_send(UART1,*data);
		data++;
	}
	uart_sendline(UART1,"\n");
}

int write_to_esp8266(char *data)
{
   	while (*data != '\0')
	{
		uart_send(ESP_UART, *data);
		data++;
	}
	//uart_sendline(ESP_UART, "\r\n");
	
}

int write_enter_to_esp8266()
{
	 uart_sendline(ESP_UART,"\r");
    uart_sendline(ESP_UART,"\n");

}


void at_command(char *input , int wait, char *data)
{
    print_char(input);
	write_to_esp8266(input);
    write_enter_to_esp8266();
	delay(wait*1000);//wait in msec
	//read_from_esp8266(data);
    //debug
   // uart_sendline(UART1,"ESP resp stat: ");
   // uart_send(UART1,(char)stat);
   // uart_sendline(UART1,"\r\n");
}
/*
void transmit_data(int temperature) {
    char data[300];
    char tempStr[10];
    char fullRequest[200];
    char cipsend_cmd[50];
    my_int2str(temperature, tempStr);
    //Building payload for http request
    my_strcpy(fullRequest,"POST /api/v2/skmaityfet/feeds/temperature/data ");
    my_strcat(fullRequest,"HTTP/1.1\r\n");
    my_strcat(fullRequest,"Host: io.adafruit.com \r\nContent-Type: application/json\r\nX-AIO-Key: ");
    my_strcat(fullRequest,API_KEY);//API key of cloud
    my_strcat(fullRequest,"\r\nContent-Length: ");
    //sprintf(data,"{\"value\": %d}",getSensorData());
    my_strcat(fullRequest,"9");//total length of data, for temp->2, format->7
    my_strcat(fullRequest,"\r\n\r\n");
    my_strcat(fullRequest,tempStr);
//
    // 2. Calculate the EXACT length
    // We add 2 because write_to_esp8266 adds its own \r\n at the very end
    int total_len = my_strlen(fullRequest) + 2;
    my_strcpy(cipsend_cmd, "AT+CIPSEND=0,");
    char lenStr[10];
    my_int2str(total_len, lenStr);//calculating total payload length
    my_strcat(cipsend_cmd, lenStr);//Equv to:sprintf(atcommand,"AT+CIPSEND=0,%d",strlen(payload));
    // Initiate ESP01 to Enable TCP connection
    at_command("AT+CIPMUX=1",1000,data);
    // 3. connect to host and port
    at_command("AT+CIPSTART=0,\"TCP\",\"io.adafruit.com\",80", 2000, data);
    //sending payload in json format
    at_command(cipsend_cmd, 1000, data); // Wait for '>'
    
    write_to_esp8266(fullRequest); // This adds the final \r\n
    
    delay(5000); // Wait for ThingSpeak to reply
    at_command("AT+CIPCLOSE=0", 1, data);
}
*/
void transmit_data(int temperature) {
    char data[200];
    char tempStr[10];
    char jsonPayload[40] ;
    char fullRequest[400];
    char cipsend_cmd[50];
    char lenStr[10];

    // 1. Create the ACTUAL JSON body
    // Result: {"value": 24}
    my_strcpy(jsonPayload, "{\"value\": ");
    my_int2str(temperature, tempStr);
    my_strcat(jsonPayload, tempStr);
    my_strcat(jsonPayload, "}");

    // 2. Build the HTTP Request
    my_strcpy(fullRequest, "POST /api/v2/skmaityfet/feeds/temperature/data HTTP/1.1\r\n");
    my_strcat(fullRequest, "Host: io.adafruit.com\r\n");
    my_strcat(fullRequest, "Content-Type: application/json\r\n");
    my_strcat(fullRequest, "X-AIO-Key: ");
    my_strcat(fullRequest, API_KEY);
    my_strcat(fullRequest, "\r\nContent-Length: ");
    my_int2str(my_strlen(jsonPayload), lenStr); 
    my_strcat(fullRequest, lenStr);
    my_strcat(fullRequest, "\r\n\r\n");
    my_strcat(fullRequest, jsonPayload);
    my_strcat(fullRequest, "Connection: close\r\n");

    // 3. Calculate length for AT+CIPSEND
    // Note: write_to_esp8266 adds \r\n (2 bytes), so we must include them
    //int total_len = my_strlen(fullRequest) + 2; 
    int total_len = my_strlen(fullRequest);
    my_strcpy(cipsend_cmd, "AT+CIPSEND=0,");
    my_int2str(total_len, lenStr);
    my_strcat(cipsend_cmd, lenStr);

    // 4. Send Commands
    at_command("AT+CIPSTART=0,\"TCP\",\"io.adafruit.com\",80", 2000, data);
    at_command(cipsend_cmd, 5000, data); 
    //debug
   // uart_sendline(UART1,fullRequest);
   // uart_sendline(UART1,"\r\n");
    delay(5000000);
    write_to_esp8266(fullRequest); 
    
    delay(10000000); 
    at_command("AT+CIPCLOSE=0", 1000, data);
}

int main()
{
    char data[200];
    int temperature = 24;
  //initialize wifi connection
    at_command("AT",1000,data);
    at_command("AT+CWMODE=1",1000,data);
    at_command("AT+CWJAP=\"realmep3\",\"12345687\"",1000,data);//putting ssid and password
    at_command("AT+CIPMUX=1", 5000, data);
   while(1){
    transmit_data(temperature);//send data to adafruit cloud
    delay(5000000);
   }
   return 0;
}