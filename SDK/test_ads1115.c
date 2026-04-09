#include <stdio.h>
#include "i2c.h"
#include "ads1115.h"
#include "delay.h"
#include "uart.h"

int main(void) {
    // 1. Initialize I2C0
    // Example: System Clock 100MHz, Target I2C 100kHz
    // Divider = (100,000,000 / (2 * 100,000)) - 1 = 498 
    i2c_init(&I2C0, 249);
    //debug test
    // Force a write of the default config, then read it back
i2c_write16(&I2C0, ADS1115_ADDR, ADS_REG_CONFIG, 0x8583);
uint8_t test = i2c_read(&I2C0, ADS1115_ADDR, ADS_REG_CONFIG);
uart_sendline(UART1,"Config MSB after 16-bit write: ");
uart_sendnumber(UART1,test);
/*
  //  ads1115_init_continuous(&I2C0);
   if(!ads1115_check_presence(&I2C0))
   while(1);//stuck in loop
   uart_sendline(UART1,"ADS1115 ADC Driver Started...\r\n");
   uart_sendline(UART1,"Range set to +/- 4.096V (0.125mV per bit)\r\n");
   uart_sendline(UART1,"----------------------------------------\r\n");

    while (1) {
        // Read the Config Register (0x01). Default should be 0x8583.
    uint8_t test_msb = i2c_read(&I2C0, ADS1115_ADDR, ADS_REG_CONFIG);
    uart_sendline(UART1,"Test Read Config MSB: ");
    uart_sendnumber(UART1,test_msb);
        // 2. Trigger a conversion on Channel A0
        ads1115_request_read(&I2C0, ADS_CH_A0);

        // 3. Wait for conversion to complete
        // ADS1115 at 128 SPS takes ~8ms. This delay is safe.
        delay(100000);

        // 4. Read the raw 16-bit value
        int16_t raw_val = ads1115_get_raw(&I2C0);

        // 5. Convert to millivolts using 32-bit fixed point math
        int32_t voltage_mv = ads1115_raw_to_mv(raw_val);

        // 7. Print Result: "Voltage:123 mV"
        uart_sendline(UART1,"Measured Volt: ");
        uart_sendnumber(UART1,voltage_mv);
        uart_sendline(UART1," mV\r\n");

        // Wait before next sample (2sec)
        delay(2000000);
    }
*/
while(1);
    return 0;
}