#include "soft_ads1115.h"
#include "gpio.h"
#include "uart.h"
#include "delay.h"

// Simple millisecond delay (adjust for your CPU frequency)
void delay_msec(int ms) {
    delay(ms*1000);
}

int main() {
    // 1. Initialize UART for debugging
    uart_init(UART1);
    uart_sendline(UART1, "--- ADS1115 Bit-Bang Start ---\r\n");

    // 2. Initialize GPIO Pins (SCL=0, SDA=1)
    // We start with pins as INPUTS to let pull-up resistors take them high
    pinMode(0, 0); //SCL is LOW
    pinMode(1, 0); //SDA PIN LOW

    // 3. Check Presence
    // We read the config register. Default is 0x8583.
    uint16_t config = soft_ads1115_read_reg16(ADS_REG_CONFIG);
    
    if ((config & 0xFF80) == 0x8580) {
        uart_sendline(UART1, "ADS1115 Detected successfully!\r\n");
    } else {
        uart_sendline(UART1, "ADS1115 NOT found. Check wiring/pull-ups.\r\n");
        uart_sendline(UART1, "Received Config: ");
        uart_sendnumber(UART1, config);
        uart_sendline(UART1, "\r\n");
        while(1); // Halt
    }


    while (1) {
        // 4. Request a single-shot conversion on Channel 0
        // OS=1, MUX=AIN0, PGA=4.096V, MODE=Single-Shot
        uint16_t request_config = ADS_OS_START | ADS_MUX_SINGLE_A0 | 
                                  ADS_PGA_4096 | ADS_MODE_SINGLE | 
                                  ADS_DR_128SPS | ADS_COMP_QUE_OFF;
        
        soft_ads1115_write_reg16(ADS_REG_CONFIG, request_config);

        // 5. Wait for conversion to complete (~8-10ms for 128SPS)
        delay_msec(50);

        // 6. Read the result and convert to mV
        int32_t voltage_mv = soft_ads1115_get_mv();

        // 7. Display Result
        uart_sendnumber(UART1, voltage_mv);
        uart_sendline(UART1, "\r\n");

        // Delay before next sample
        delay_msec(500);
    }

    return 0;
}
