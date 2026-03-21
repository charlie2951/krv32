#ifndef THINGSPEAK_H
#define THINGSPEAK_H

#include "esp_wifi.h"

// Status codes for debugging
typedef enum {
    TS_SUCCESS = 0,
    TS_ERR_CONNECT,
    TS_ERR_SEND,
    TS_ERR_TIMEOUT
} TS_Status;

/**
 * @brief Sends data to ThingSpeak Cloud
 * @param api_key: Your unique ThingSpeak Write API Key
 * @param data: Array of integer values to send
 * @param sensor_count: Number of fields to update (1-8)
 * @return TS_Status result of the operation
 */
TS_Status ThingSpeak_Send(const char* api_key, int data[], int sensor_count);

#endif