#include "esp_wifi.h"
#include "printf.h" // For my_printf (logging)
#include "uart.h"
#include "delay.h"
#define ESP_UART UART2 //uart2 is used to coonect ESP01 wifi module

static char cmd_buf[ESP_BUF_SIZE];

/**
 * @brief Sends a formatted command to the ESP via UART
 */
void ESP_SendCommand(const char* cmd) {
    // We use your custom print_string or UART_PutChar here
    while (*cmd) {
        uart_send(ESP_UART,(uint8_t)*cmd++);
    }
    //sending /r/n after every command
    uart_sendline(ESP_UART,"\r\n");
}
//write enter to esp01-needed during tcp payload
void ESP_write_enter(void) {
      uart_sendline(ESP_UART,"\r\n");
}
/**
 * @brief Simple delay for timeout (replace with a real timer/tick if available)
 */
static void simple_delay(unsigned int ms) {
    for (volatile unsigned int i = 0; i < ms * 1000; i++); 
}

/**
 * @brief Scans incoming UART data for a specific success string
 */
int ESP_WaitResponse(const char* expected, unsigned int timeout_ms) {
    // In a real bare-metal app, you'd check a circular UART buffer here.
    // For this example, we assume 'UART_RxBuffer' is populated by an ISR.
    //currently KRV-32 doesnt ISR, hence this func will not work
    //extern char UART_RxBuffer[]; 
    char UART_RxBuffer[100];
    unsigned int elapsed = 0;
    while (elapsed < timeout_ms) {
        if (my_strstr(UART_RxBuffer, expected)) {
            my_memset(UART_RxBuffer, 0, 100); // Clear buffer after match
            return 1; // Success
        }
        simple_delay(10);
        elapsed += 10;
    }
    return 0; // Timeout
}

/**
 * @brief Set Mode (1: Station, 2: AP, 3: Both)
 */
int ESP_SetMode(int mode) {
    my_sprintf(cmd_buf, "AT+CWMODE=%d", mode);
    ESP_SendCommand(cmd_buf);
   // return ESP_WaitResponse("OK", 1000);
   return 1;
}

/**
 * @brief Connect to external Router
 */
int ESP_ConnectToAP(const char* ssid, const char* pass) {
    my_sprintf(cmd_buf, "AT+CWJAP=\"%s\",\"%s\"", ssid, pass);
    ESP_SendCommand(cmd_buf);
    //return ESP_WaitResponse("WIFI GOT IP", 10000); // 10s timeout for Wi-Fi
  return 1;
}

/**
 * @brief Setup the ESP's own Hotspot
 */
int ESP_ConfigureSoftAP(const char* ssid, const char* pass) {
    // AT+CWSAP="SSID","PASS",Channel,Encryption
    my_sprintf(cmd_buf, "AT+CWSAP=\"%s\",\"%s\",5,3", ssid, pass);
    ESP_SendCommand(cmd_buf);
   // return ESP_WaitResponse("OK", 2000);
   return 1;
}

void ESP_Init(void) {
    printf("Initializing ESP...\n");
    ESP_SendCommand("AT");
    /*
    if (ESP_WaitResponse("OK", 500)) {
        printf("ESP Detected!\n");
    } else {
        //printf("ESP Not Responding.\n");
        printf("...\r\n");
    }*/
}
/**
 * @brief Establishes a TCP connection
 */
int ESP_TCP_Connect(int link_id, const char* type, const char* ip, int port) {
    // 1. Ensure Multiple Connections are enabled (required for most apps)
    ESP_SendCommand("AT+CIPMUX=0");
    //ESP_WaitResponse("OK", 500);
    delay(5000);
    // 2. Format: AT+CIPSTART=0,"TCP","192.168.1.5",80
    my_sprintf(cmd_buf, "AT+CIPSTART=\"%s\",\"%s\",%d",type, ip, port);
     ESP_SendCommand(cmd_buf);
    delay(1000);
    // Wait for "CONNECT" or "OK"
   // return ESP_WaitResponse("CONNECT", 5000); 
   return 1;
}

/**
 * @brief Sends data over an established link
 */
int ESP_Send_Data(int link_id, const char* data) {
    unsigned int len = my_strlen(data)+2;
    printf("\r\nAT+CIPSEND=%d",len);
    // 1. Tell ESP how much data is coming
    my_sprintf(cmd_buf, "AT+CIPSEND=%d", len);
    ESP_SendCommand(cmd_buf);
    delay(1000000);
    // 2. Wait for the '>' prompt
    //if (ESP_WaitResponse(">", 1000)) {
        // 3. Send the actual payload
        ESP_SendCommand(data);
      //  return ESP_WaitResponse("SEND OK", 2000);
   // }
    
  //  return 0; // Failed to get prompt
  return 1;
}