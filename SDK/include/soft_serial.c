#include "soft_serial.h"
#include "gpio.h"
#include "timer.h"

#define TIMER TIMER1 //using timer1 for count

void  delayMicroseconds(uint32_t value){
timer_delay_us(TIMER,value);
}

void soft_serial_begin(SoftSerial* dev, uint32_t baud, uint8_t tx, uint8_t rx) {
    dev->tx_pin = tx;
    dev->rx_pin = rx;
    
    // Calculate period and subtract overhead for better accuracy
    dev->bit_period_us = (1000000 / baud) - OVERHEAD_US;

    pinMode(dev->tx_pin, OUTPUT); // Output mode
    pinMode(dev->rx_pin, INPUT); // Input mode
    
    digitalWrite(dev->tx_pin, OUTPUT); // UART Idle is HIGH
}

void soft_serial_write(SoftSerial* dev, uint8_t byte) {
    // Start bit (Low)
    digitalWrite(dev->tx_pin, INPUT);
    delayMicroseconds(dev->bit_period_us);

    // 8 Data bits (LSB first)
    for (uint8_t i = 0; i < 8; i++) {
        digitalWrite(dev->tx_pin, (byte >> i) & 0x01);
        delayMicroseconds(dev->bit_period_us);
    }

    // Stop bit (High)
    digitalWrite(dev->tx_pin, OUTPUT);
    delayMicroseconds(dev->bit_period_us);
}

int soft_serial_read(SoftSerial* dev) {
    // If line is HIGH, no start bit is present
    if (digitalRead(dev->rx_pin) == 1) {
        return -1;
    }

    // Wait 1.5 bit periods to sample right in the center of the first data bit
    delayMicroseconds(dev->bit_period_us + (dev->bit_period_us / 2));

    uint8_t data = 0;
    for (uint8_t i = 0; i < 8; i++) {
        if (digitalRead(dev->rx_pin)) {
            data |= (1 << i);
        }
        delayMicroseconds(dev->bit_period_us);
    }

    // Wait for the stop bit period to end before returning
    delayMicroseconds(dev->bit_period_us / 2);
    
    return (int)data;
}

uint8_t soft_serial_busy(SoftSerial* dev) {
    // In UART, the line drops LOW to signal the start of a byte
    // If it's LOW, the ESP-01 is currently pushing data
    if(digitalRead(dev->rx_pin) == 0)
    return 1;
    else return 0;
}
/**
 * Writes a null-terminated string to the SoftSerial port.
 */
void soft_serial_print(SoftSerial* dev, const char* str) {
    if (str == 0) return; // Safety check for null pointers
    
    while (*str != '\0') {
        soft_serial_write(dev, (uint8_t)(*str));
        str++; // Move to the next character
    }
}

/**
 * Writes a string followed by a Carriage Return and Line Feed (\r\n).
 * Essential for ESP-01 AT commands.
 */
void soft_serial_println(SoftSerial* dev, const char* str) {
    soft_serial_print(dev, str);
    soft_serial_write(dev, '\r');
    soft_serial_write(dev, '\n');
}