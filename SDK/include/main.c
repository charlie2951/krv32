#include "uart.h"
#include "led_driver.h"
#include "delay.h"

int main() {
    // 1. Signal system start on LEDs
    led_write(0xFF); 
    delay(5000000);
    led_write(0x00);

    // 2. Send welcome message via UART1
    uart_sendline(UART1, "RISC-V Bootloader Ready...\n");

    while(1) {
        uint32_t data = uart_receive(UART1);
        
        // 3. Display received byte on LEDs
        led_write(data);
        
        // 4. Echo back to UART2 for monitoring
      //  uart_sendline(UART2, "Data mirrored to UART2\n");
    }

    return 0;
}