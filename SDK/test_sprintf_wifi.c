#include "my_string.h"
#include "uart.h"
#include "delay.h"
#include "printf.h"

#define ESP_UART UART2

int main()
{
char atcommand[128];
int channel = 6;
char *ssid = "Airtel_subir_airtel";
char *pass = "kiit2951";
printf("custom printf library test\r\n");
printf("Using ssid=%s and password=%s",ssid,pass);

my_memset(atcommand,0,128);
my_sprintf(atcommand, "AT+CWJAP=\"%s\",%d\r\n", ssid, channel);
uart_sendline(ESP_UART,atcommand);
delay(1000000);
my_memset(atcommand,0,128);
my_sprintf(atcommand,"AT+CWMODE=3\r\n");
uart_sendline(ESP_UART,atcommand);
delay(1000000);

// Result: AT+CWJAP="MyHomeWiFi",6
while(1);

}
