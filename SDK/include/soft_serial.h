#ifndef SOFT_SERIAL_H
#define SOFT_SERIAL_H

#include <stdint.h>


// Tuning constant to account for function call overhead on RISC-V
#define OVERHEAD_US 2 

typedef struct {
    uint8_t tx_pin;
    uint8_t rx_pin;
    uint32_t bit_period_us;
} SoftSerial;

void soft_serial_begin(SoftSerial* dev, uint32_t baud, uint8_t tx, uint8_t rx);
void soft_serial_write(SoftSerial* dev, uint8_t byte);
int  soft_serial_read(SoftSerial* dev);
uint8_t soft_serial_busy(SoftSerial* dev);
void soft_serial_print(SoftSerial* dev, const char* str);
void soft_serial_println(SoftSerial* dev, const char* str);

#endif