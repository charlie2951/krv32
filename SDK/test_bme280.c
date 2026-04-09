#include "spi.h"
#include "bme280_spi.h"
#include "printf.h"
#include "delay.h"
int main() {
    int32_t  temperature;
    uint32_t pressure;

    // Initialize SPI with divider for desired speed
    if (bme280_init_spi(10) == 0) { 
        while(1) {
            // get_temperature triggers the forced measurement internally
            bme280_get_temperature_spi(&temperature);
            bme280_get_pressure_spi(&pressure);

            printf("SPI Temp: %d.%d C | Press: %u.%u hPa\r\n", 
                    temperature / 100, temperature % 100, 
                    pressure / 100, pressure % 100);

            delay(8000000);
        }
    }
    return 0;
}