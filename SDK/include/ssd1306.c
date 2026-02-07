#include "ssd1306.h"
#include "softi2c.h"
#include "font5x7_full.h"   // standard 5x7 font

static void ssd1306_cmd(uint8_t cmd)
{
    softi2c_start();
    softi2c_write_byte(SSD1306_ADDR << 1);
    softi2c_write_byte(0x00);   // command
    softi2c_write_byte(cmd);
    softi2c_stop();
}

static void ssd1306_data(uint8_t data)
{
    softi2c_start();
    softi2c_write_byte(SSD1306_ADDR << 1);
    softi2c_write_byte(0x40);   // data
    softi2c_write_byte(data);
    softi2c_stop();
}

void ssd1306_init(void)
{
    ssd1306_cmd(0xAE); // display off
    ssd1306_cmd(0x20); // memory mode
    ssd1306_cmd(0x00); // horizontal
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
    ssd1306_cmd(0xAF); // display ON

    ssd1306_clear();
}

void ssd1306_set_cursor(uint8_t page, uint8_t col)
{
    ssd1306_cmd(0xB0 + page);
    ssd1306_cmd(0x00 + (col & 0x0F));
    ssd1306_cmd(0x10 + (col >> 4));
}

void ssd1306_clear(void)
{
    for (uint8_t page = 0; page < 8; page++) {
        ssd1306_set_cursor(page, 0);
        for (uint8_t col = 0; col < 128; col++)
            ssd1306_data(0x00);
    }
}

//font indext to translate ASCII to font Index
int font_index(char c)
{
    if (c == '!') return 0;
    if (c == '#') return 1;
    if (c == '.') return 2;
    if (c >= '0' && c <= '9') return 3 + (c - '0');
    if (c >= 'A' && c <= 'Z') return 13 + (c - 'A');
    return -1;   // unsupported
}

void ssd1306_write_char(char c)
{
    if (c < 32 || c > 127) c = '?';

    for (int i = 0; i < 5; i++)
        ssd1306_data(font5x7[c - 32][i]);

    ssd1306_data(0x00); // spacing
}

/*
void ssd1306_write_char(char c)
{
    int idx = font_index(c);
    if (idx < 0) return;

    for (int i = 0; i < 5; i++)
        ssd1306_data(font5x7[idx][i]);

    ssd1306_data(0x00);   // spacing
}
    */

void ssd1306_write_string(const char *str)
{
    while (*str)
        ssd1306_write_char(*str++);
}
