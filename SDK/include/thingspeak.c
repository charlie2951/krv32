#include "thingspeak.h"
#include "printf.h"
#include "delay.h"

TS_Status ThingSpeak_Send(const char* api_key, int data[], int sensor_count) {
    char payload[300]; // Buffer for the full HTTP request
    char field_buf[32]; // Temporary buffer for each field string
    
    if (sensor_count > 8) sensor_count = 8;

    // 1. Build the base of the URL
    // Format: GET /update?api_key=XXXXXXXXXXXXXXXX
    my_sprintf(payload, "GET https://api.thingspeak.com/update?api_key=%s", api_key);

    // 2. Dynamically append fields: &field1=10&field2=20...
    for (int i = 0; i < sensor_count; i++) {
        my_sprintf(field_buf, "&field%d=%d", i + 1, data[i]);
        my_strcat(payload, field_buf);
    }
    //3. append /r/n /r/n at the end of payload
    // my_strcat(payload, "/r/n");
    //print the payload for debugging
    printf("\r\nPayload is:%s",payload);
    
     printf("\r\nTS: Connecting to Cloud...\n");

    // 4. Execute using your esp_wifi library
    // Link ID 0 is used for the TCP connection
    ESP_TCP_Connect(0, "TCP", "api.thingspeak.com", 80);
    delay(1000000);
    printf("TS: Sending Payload...\n\r");
    ESP_Send_Data(0, payload);
    ESP_write_enter();
    delay(5000000);
    ESP_SendCommand("AT+CIPCLOSE=0"); // Cleanup
       return 1;
    

    // 5. Cleanup and Close
    printf("TS: Data Sent Successfully.\n");
    ESP_SendCommand("AT+CIPCLOSE=0\r\n");
  //  ESP_WaitResponse("OK", 1000);

    return 1;
}