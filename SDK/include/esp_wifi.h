#ifndef ESP_WIFI_H
#define ESP_WIFI_H

#include "my_string.h"

#define TCP "TCP"
#define UDP "UDP"

// Buffer size for AT commands
#define ESP_BUF_SIZE 128

// Function Prototypes
void ESP_Init(void);
int  ESP_SetMode(int mode);
int  ESP_ConnectToAP(const char* ssid, const char* pass);
int  ESP_ConfigureSoftAP(const char* ssid, const char* pass);
void ESP_SendCommand(const char* cmd);
int  ESP_WaitResponse(const char* response, unsigned int timeout_ms);
int ESP_TCP_Connect(int link_id, const char* type, const char* ip, int port);
int ESP_Send_Data(int link_id, const char* data);
void ESP_write_enter(void);
#endif