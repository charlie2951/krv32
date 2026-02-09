#ifndef TM1637_H
#define TM1637_H

#include <stdint.h>

/* Configuration Structure */
typedef struct {
    uint8_t dio_pin;
    uint8_t clk_pin;
} tm1637_config_t;

/* Library Initialization */
void tm_init(const tm1637_config_t *config);

/* Display Functions */
void tm_display_string(const char *str);
void tm_display_num(int num);
void tm_display_clock(int h, int m, int show_colon);
void tm_display_scroll(const char *text, uint32_t speed_ms);

/* Low-level Protocol (if needed externally) */
void tm_start(void);
void tm_stop(void);
void tm_write_byte(uint8_t b);
uint8_t ascii_to_seg(char c);

#endif // TM1637_H