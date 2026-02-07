#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>

#define SSD1306_ADDR  0x3C

void ssd1306_init(void);
void ssd1306_clear(void);
void ssd1306_set_cursor(uint8_t page, uint8_t col);
void ssd1306_write_char(char c);
void ssd1306_write_string(const char *str);

#endif
