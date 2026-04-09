#include "i2c.h"
#include "bme280.h"
#include "printf.h"
#include "delay.h"
#include "spi.h"//use it if using spi interface
int main() {
    int32_t  temperature;
    uint32_t pressure;
    uint32_t humidity;

    // Initialize I2C (adjust divider for your clock frequency)
    //i2c_init(&I2C0, 499); 
   // uint8_t resp = bme280_init_i2c(&I2C0);
    // For SPI use following
    uint8_t resp = bme280_init_spi(10);
    // Initialize BME280
    if ( resp == 0) { 
        printf("BME280 Initialized Successfully!\r\n");
        
        while(1) {
            // 1. Trigger a fresh measurement (Forced Mode)
            // This is necessary because the sensor sleeps after every reading
          //  bme280_i2c_trigger_forced(&I2C0);//use it if using forced mode in i2c

            // 2. Read the compensated values
            // Note: bme280_get_temperature MUST be called first to update t_fine
          //  bme280_i2c_get_temperature(&I2C0, &temperature);
          //  bme280_i2c_get_pressure(&I2C0, &pressure);
            //bme280_get_humidity(&I2C0, &humidity);
            // For SPI, use the following
            bme280_get_temperature_spi(&temperature);
            bme280_get_pressure_spi(&pressure);
            // 3. Print the results
            // Temperature is returned as (Celsius * 100)
            // Pressure is returned as (hPa)
            // Humidity is returned as (%RH * 1024)
            printf(" BME280 -> Temp: %d.%d C | Press: %d.%d hPa \r\n", 
                    temperature / 100,  temperature % 100, 
                    pressure/100, pressure%100);

            // Delay for readability (adjust based on your clock speed)
            delay(8000000);
        }
    } else {
        printf("Error: BME280 not found!\r\n");
    }

    return 0;
}