#ifndef MCP4725_H
#define MCP4725_H

#include "i2c.h"

// Command Bits
#define MCP4725_CMD_WRITEDAC         0x40  // Write to DAC register
#define MCP4725_CMD_WRITEDACEEPROM   0x60  // Write to DAC and EEPROM (persists after power cycle)
#define MCP4725_ADDR 0x62 // Check if your ADDR pin is tied to GND (0x60) or VCC (0x61)
//API
void mcp4725_init(i2c_t *i2c,uint16_t DIV_CLK);
void mcp4725_set_voltage(i2c_t *i2c, uint16_t value, uint8_t store_in_eeprom);
void mcp4725_set_voltage_fast(i2c_t *i2c, uint16_t value);

#endif