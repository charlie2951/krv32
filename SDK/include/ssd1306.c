#include "ssd1306.h"
#include "font5x7_full.h"

// ------------------------------------------------------
// Private driver state
// ------------------------------------------------------
static i2c_t *ssd_i2c = 0;   // stored I2C instance

// ------------------------------------------------------
// Low level helpers
// ------------------------------------------------------
static void ssd1306_cmd(uint8_t cmd)
{
    i2c_write(ssd_i2c, SSD1306_ADDR, 0x00, cmd);
}

static void ssd1306_data(uint8_t data)
{
    i2c_write(ssd_i2c, SSD1306_ADDR, 0x40, data);
}

// ------------------------------------------------------
// Initialization
// ------------------------------------------------------
void ssd1306_init(i2c_t *i2c)
{
  //  i2c_init(i2c, 499);//100khz i2c speed
   i2c_init(i2c, 124);//400khz i2c speed
    ssd_i2c = i2c;   // store selected bus

    ssd1306_cmd(0xAE);
    ssd1306_cmd(0x20);
    ssd1306_cmd(0x00);
    ssd1306_cmd(0xB0);
    ssd1306_cmd(0xC8);
    ssd1306_cmd(0x00);
    ssd1306_cmd(0x10);
    ssd1306_cmd(0x40);
    ssd1306_cmd(0x81);
    ssd1306_cmd(0x7F);
    ssd1306_cmd(0xA1);
    ssd1306_cmd(0xA6);
    ssd1306_cmd(0xA8);
    ssd1306_cmd(0x3F);
    ssd1306_cmd(0xA4);
    ssd1306_cmd(0xD3);
    ssd1306_cmd(0x00);
    ssd1306_cmd(0xD5);
    ssd1306_cmd(0x80);
    ssd1306_cmd(0xD9);
    ssd1306_cmd(0xF1);
    ssd1306_cmd(0xDA);
    ssd1306_cmd(0x12);
    ssd1306_cmd(0xDB);
    ssd1306_cmd(0x40);
    ssd1306_cmd(0x8D);
    ssd1306_cmd(0x14);
    ssd1306_cmd(0xAF);

    ssd1306_clear();
}

// ------------------------------------------------------
// Cursor
// ------------------------------------------------------
void ssd1306_set_cursor(uint8_t page, uint8_t col)
{
    ssd1306_cmd(0xB0 + page);
    ssd1306_cmd(0x00 + (col & 0x0F));
    ssd1306_cmd(0x10 + (col >> 4));
}

// ------------------------------------------------------
// Clear display
// ------------------------------------------------------
void ssd1306_clear(void)
{
    for (uint8_t page = 0; page < 8; page++)
    {
        ssd1306_set_cursor(page, 0);

        for (uint8_t col = 0; col < 128; col++)
            ssd1306_data(0x00);
    }
}

// ------------------------------------------------------
// Character
// ------------------------------------------------------
void ssd1306_write_char(char c)
{
    if (c < 32 || c > 127)
        c = '?';

    for (int i = 0; i < 5; i++)
        ssd1306_data(font5x7[c - 32][i]);

    ssd1306_data(0x00);
}

// ------------------------------------------------------
// String
// ------------------------------------------------------
void ssd1306_write_string(const char *str)
{
    while (*str)
        ssd1306_write_char(*str++);
}
