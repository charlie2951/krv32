#include "tm1637.h"
#include "delay.h"

/* GPIO MMIO */
#define GPIO_DATA (*(volatile uint32_t *)0x10000000)
#define GPIO_DIR  (*(volatile uint32_t *)0x10000004)

/* Pointer to store user-defined pin configuration */
static const tm1637_config_t *tm_cfg;

/* Modified GPIO helpers using structure values */
static inline void dio_out(void)  { GPIO_DIR |=  (1 << tm_cfg->dio_pin); }
static inline void dio_in(void)   { GPIO_DIR &= ~(1 << tm_cfg->dio_pin); }
static inline void dio_high(void) { GPIO_DATA |=  (1 << tm_cfg->dio_pin); }
static inline void dio_low(void)  { GPIO_DATA &= ~(1 << tm_cfg->dio_pin); }
static inline void clk_high(void) { GPIO_DATA |=  (1 << tm_cfg->clk_pin); }
static inline void clk_low(void)  { GPIO_DATA &= ~(1 << tm_cfg->clk_pin); }

/* Expanded Font Map: 0-9, A-Z */
static const uint8_t alpha_seg_map[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, // 0-9
    0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x3D, 0x76, 0x06, 0x1E, // A, b, C, d, E, F, G, H, I, J
    0x76, 0x38, 0x15, 0x37, 0x3F, 0x73, 0x67, 0x50, 0x6D, 0x78, // K, L, M, n, O, P, q, r, S, t
    0x3E, 0x1C, 0x2A, 0x76, 0x6E, 0x5B                          // U, v, W, X, y, Z
};

void tm_init(const tm1637_config_t *config) {
    tm_cfg = config;
    GPIO_DIR |= (1 << tm_cfg->dio_pin) | (1 << tm_cfg->clk_pin);
    dio_high();
    clk_high();
}

void tm_start(void) {
    dio_out(); dio_high(); clk_high(); delay(200); dio_low();
}

void tm_stop(void) {
    dio_out(); clk_low(); delay(200); dio_low(); delay(200);
    clk_high(); delay(200); dio_high();
}

void tm_write_byte(uint8_t b) {
    dio_out();
    for (int i = 0; i < 8; i++) {
        clk_low(); delay(200);
        if (b & 0x01) dio_high(); else dio_low();
        delay(200); clk_high(); delay(200);
        b >>= 1;
    }
    /* ACK sequence */
    clk_low(); dio_in(); delay(200);
    clk_high(); delay(200); clk_low();
    dio_out();
}

uint8_t ascii_to_seg(char c) {
    if (c >= '0' && c <= '9') return alpha_seg_map[c - '0'];
    if (c >= 'A' && c <= 'Z') return alpha_seg_map[c - 'A' + 10];
    if (c >= 'a' && c <= 'z') return alpha_seg_map[c - 'a' + 10];
    if (c == '-') return 0x40;
    if (c == ' ') return 0x00;
    return 0x00;
}

void tm_display_string(const char *str) {
    tm_start();
    tm_write_byte(0x40); 
    tm_stop();
    tm_start();
    tm_write_byte(0xC0);
    for (int i = 0; i < 4; i++) {
        if (str[i] == '\0') tm_write_byte(0x00);
        else tm_write_byte(ascii_to_seg(str[i]));
    }
    tm_stop();
    tm_start();
    tm_write_byte(0x8F);
    tm_stop();
}

void tm_display_num(int num) {
    char buf[5];
    for (int i = 3; i >= 0; i--) {
        buf[i] = (num % 10) + '0';
        num /= 10;
    }
    buf[4] = '\0';
    tm_display_string(buf);
}

void tm_display_clock(int h, int m, int show_colon) {
    uint8_t h1 = (h < 10) ? 0x00 : ascii_to_seg((h / 10) + '0');
    uint8_t h2 = ascii_to_seg((h % 10) + '0');
    if (show_colon) h2 |= 0x80;
    
    tm_start();
    tm_write_byte(0x40); tm_stop();
    tm_start();
    tm_write_byte(0xC0);
    tm_write_byte(h1);
    tm_write_byte(h2);
    tm_write_byte(ascii_to_seg((m / 10) + '0'));
    tm_write_byte(ascii_to_seg((m % 10) + '0'));
    tm_stop();
    tm_start();
    tm_write_byte(0x8A); // Brightness 10/16
    tm_stop();
}

void tm_display_scroll(const char *text, uint32_t speed_ms) {
    int len = 0;
    while (text[len] != '\0') len++;
    for (int i = 0; i < len + 4; i++) {
        tm_start();
        tm_write_byte(0x40); tm_stop();
        tm_start();
        tm_write_byte(0xC0);
        for (int j = 0; j < 4; j++) {
            int idx = i - 3 + j;
            if (idx >= 0 && idx < len) tm_write_byte(ascii_to_seg(text[idx]));
            else tm_write_byte(0x00);
        }
        tm_stop();
        tm_start();
        tm_write_byte(0x8F); tm_stop();
        delay(speed_ms * 1000);
    }
}