#ifndef DHT11_H
#define DHT11_H

#include <stdint.h>
#include "gpio.h"
#include "timer.h"

/* -------------------------------------------------
 * Error codes (not implemented till now)
 * ------------------------------------------------- */
#define DHT11_ERROR_CHECKSUM   254
#define DHT11_ERROR_TIMEOUT    253

#define DHT11_TIMEOUT_MS       5000

/* -------------------------------------------------
 * DHT11 device structure
 * ------------------------------------------------- */
typedef struct
{
    int pin;                /* GPIO pin number */
    uint32_t delay_ms;      /* Delay between reads */
} dht11_t;

static int dht_pin;

//API style
void dht11_init(uint8_t pin);
int dht11_read_temperature_humidity(uint32_t *temperature, uint32_t *humidity);
#endif