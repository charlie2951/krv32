#include "mcp4725.h"
#include "i2c.h"
#include <stdint.h>

//Initialize the sensor
void mcp4725_init(i2c_t *i2c,uint16_t DIV_CLK){
    // 1. Initialize I2C0
    // Example: For a 100MHz clock and 100kHz I2C:
    // divider = (100,000,000 / (2 * 100,000)) - 1 = 499
    i2c_init(&I2C0, DIV_CLK); //setting up i2c  at 100KHz
}
/**
 * Sets the output voltage of the DAC.
 * @param i2c: Pointer to the I2C instance (I2C0 or I2C1)
 * @param value: 12-bit value (0 to 4095)
 * @param store_in_eeprom: If true, the value is saved as the power-on default
 */
void mcp4725_set_voltage(i2c_t *i2c, uint16_t value, uint8_t store_in_eeprom) {
    if (value > 4095) value = 4095;

    // The MCP4725 expects 3 bytes for a "Fast Write" or "Normal Write"
    // Since your i2c_write handles [Device], [Reg], [Data], we will:
    // 1. Treat the Command Byte as the 'Register'
    // 2. Treat the MSB of the data as the 'Data'
    // 3. Manually send the LSB if your hardware supports multi-byte bursts, 
    //    otherwise we adapt to your specific i2c_write signature.

    uint8_t cmd = store_in_eeprom ? MCP4725_CMD_WRITEDACEEPROM : MCP4725_CMD_WRITEDAC;
    
    // Most I2C controllers in this style auto-increment. 
    // We send the MSB first: (D11 D10 D9 D8 D7 D6 D5 D4)
    uint8_t msb = (value >> 4) & 0xFF;
    // Then LSB: (D3 D2 D1 D0 X X X X)
    uint8_t lsb = (value << 4) & 0xFF;

    // Using your existing library's write structure:
    // Note: If your i2c_write only sends ONE data byte, you may need a 
    // modified version of i2c_write to send the second data byte (LSB).
    
    i2c_write(i2c, MCP4725_ADDR, cmd, msb);
    
    // Hack: If your hardware doesn't support sequential writes automatically,
    // we might need to send the LSB separately, but usually, the DAC 
    // interprets the next byte on the bus as the LSB.
    I2C_REG(i2c, I2C_REG_MOSI) = lsb; 
    i2c_wait_while_busy(i2c);
}

/**
 * Sets the DAC voltage using Fast Mode (12-bit).
 * This fits perfectly into your i2c_write(device, reg, data) structure.
 * * @param i2c: Pointer to your I2C instance
 * @param value: 0 to 4095
 */
void mcp4725_set_voltage_fast(i2c_t *i2c, uint16_t value) {
    if (value > 4095) value = 4095;

    // Byte 1 (The "Reg" byte in your function)
    // Bits: [0][0] (Fast Mode) | [0][0] (PD bits) | [D11][D10][D9][D8]
    uint8_t reg_byte = (uint8_t)((value >> 8) & 0x0F); 
    
    // Byte 2 (The "Data" byte in your function)
    // Bits: [D7][D6][D5][D4][D3][D2][D1][D0]
    uint8_t data_byte = (uint8_t)(value & 0xFF);

    // This results in: [ADDR] -> [0000 + Upper 4 bits] -> [Lower 8 bits]
    i2c_write(i2c, MCP4725_ADDR, reg_byte, data_byte);
}