#include <stdint.h>
#include <stdbool.h>
#include "i2c.h"
#include "delay.h"
#include "mcp4725.h"
#include "soft_ads1115.h"
#include "gpio.h"
#include "uart.h"

/* Demo program send waveform pattern from DAC and convert the 
same by using ADC ADS1115 and display result in serial terminal
*/
int main(void) {
    // 2. Initialize GPIO Pins (SCL=0, SDA=1)
    // We start with pins as INPUTS to let pull-up resistors take them high
    pinMode(0, 0); //SCL is LOW
    pinMode(1, 0); //SDA PIN LOW

    // 1. Initialize I2C0
    // Example: For a 100MHz clock and 100kHz I2C:
    // divider = (100,000,000 / (2 * 100,000)) - 1 = 499
    mcp4725_init(&I2C0, 499);
    uint16_t dac_value = 0;
    
    //configure ADC
        uint16_t request_config = ADS_OS_START | ADS_MUX_SINGLE_A0 | 
                                  ADS_PGA_4096 | ADS_MODE_SINGLE | 
                                  ADS_DR_128SPS | ADS_COMP_QUE_OFF;
         soft_ads1115_write_reg16(ADS_REG_CONFIG, request_config);
        delay(10000);

    while (1) {
        /*
        //configure ADC
        uint16_t request_config = ADS_OS_START | ADS_MUX_SINGLE_A0 | 
                                  ADS_PGA_4096 | ADS_MODE_SINGLE | 
                                  ADS_DR_128SPS | ADS_COMP_QUE_OFF;
         soft_ads1115_write_reg16(ADS_REG_CONFIG, request_config);
        delay(10000);
*/
        // 2. Output the current value to the DAC
        mcp4725_set_voltage_fast(&I2C0, dac_value);
        // 6. Read the ADC result and convert to mV
        int32_t voltage_mv = soft_ads1115_get_mv();
         
        // 3. Increment the value for a sawtooth effect
        dac_value++;
        
        // Reset to 0 after reaching 12-bit max (4095)
        if (dac_value > 4095) {
            dac_value = 0;
        }
        //display result
        uart_sendnumber(UART1, voltage_mv);
        uart_sendline(UART1, "\r\n");
        // Optional: Small delay to make the ramp visible on slower tools
        delay(10000); 
    }

    return 0;
}