#include "gpio.h" // Assuming you have a GPIO driver

#define SCL_PIN 0
#define SDA_PIN 1

// Simple delay to control I2C speed (~100kHz)
void soft_i2c_delay() {
    for (volatile int i = 0; i < 8; i++); 
}

void soft_i2c_sda_high() {
    pinMode(SDA_PIN, 0); // Let pull-up pull it high
    soft_i2c_delay();
}

void soft_i2c_sda_low() {
    pinMode(SDA_PIN, 1);
    digitalWrite(SDA_PIN, 0);
    soft_i2c_delay();
}

void soft_i2c_scl_high() {
    pinMode(SCL_PIN, 0);
    soft_i2c_delay();
}

void soft_i2c_scl_low() {
    pinMode(SCL_PIN, 1);
    digitalWrite(SCL_PIN, 0);
    soft_i2c_delay();
}

// --- Protocol Functions ---

void soft_i2c_start() {
    soft_i2c_sda_high();
    soft_i2c_scl_high();
    soft_i2c_sda_low();  // SDA falls while SCL is high
    soft_i2c_scl_low();
}

void soft_i2c_stop() {
    soft_i2c_sda_low();
    soft_i2c_scl_high();
    soft_i2c_sda_high(); // SDA rises while SCL is high
}

int soft_i2c_write_byte(uint8_t byte) {
    for (int i = 0; i < 8; i++) {
        if (byte & 0x80) soft_i2c_sda_high();
        else soft_i2c_sda_low();
        soft_i2c_scl_high();
        soft_i2c_scl_low();
        byte <<= 1;
    }
    // Check ACK
    soft_i2c_sda_high(); 
    soft_i2c_scl_high();
    int ack = !digitalRead(SDA_PIN); // ACK is active low
    soft_i2c_scl_low();
    return ack;
}

uint8_t soft_i2c_read_byte(int send_ack) {
    uint8_t byte = 0;
    soft_i2c_sda_high();
    for (int i = 0; i < 8; i++) {
        soft_i2c_scl_high();
        byte <<= 1;
        if (digitalRead(SDA_PIN)) byte |= 1;
        soft_i2c_scl_low();
    }
    // Send ACK/NACK
    if (send_ack) soft_i2c_sda_low();
    else soft_i2c_sda_high();
    soft_i2c_scl_high();
    soft_i2c_scl_low();
    return byte;
}