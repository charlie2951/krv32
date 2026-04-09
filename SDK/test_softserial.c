#include "soft_serial.h"
#include "delay.h"
#include "uart.h"
// Example GPIO numbers (Change these to your actual pins)
#define TX_PIN 0
#define RX_PIN 1
#define BAUD 115200
int main(void) {
    
    SoftSerial esp01;
    
    // Initialize at 9600 baud (common ESP-01 default)
    soft_serial_begin(&esp01, BAUD, TX_PIN, RX_PIN);

    // Sending a command is now much cleaner:
    soft_serial_println(&esp01, "AT");
    soft_serial_println(&esp01, "AT+CWMODE=3");
    // You can also send specific settings
   // soft_serial_println(&esp01, "AT+CWMODE=1");

    while(1) {
        if (soft_serial_busy(&esp01)) {
            int c = soft_serial_read(&esp01);
            // Process incoming data...
            uart_send(UART1,(char)c);
        }
    }
    
    return 0;
}