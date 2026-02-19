#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/* =========================================================
   Base Address
   ========================================================= */
#define TIMER_BASE      0xB0000000UL

/* =========================================================
   Register Offsets
   ========================================================= */
#define TIMER_CTRL_OFFSET       0x00
#define TIMER_TIMER_VAL_OFFSET  0x04
#define TIMER_COUNT_VAL_OFFSET  0x08
#define TIMER_STATUS_OFFSET     0x0C

/* =========================================================
   Register Access Macros
   ========================================================= */
#define TIMER_REG(offset) (*(volatile uint32_t *)(TIMER_BASE + (offset)))

#define TIMER_CTRL       TIMER_REG(TIMER_CTRL_OFFSET)
#define TIMER_TIMER_VAL  TIMER_REG(TIMER_TIMER_VAL_OFFSET)
#define TIMER_COUNT_VAL  TIMER_REG(TIMER_COUNT_VAL_OFFSET)
#define TIMER_STATUS     TIMER_REG(TIMER_STATUS_OFFSET)

/* =========================================================
   Bit Definitions
   ========================================================= */
#define TIMER_CTRL_START     (1u << 0)
#define TIMER_CTRL_MODE      (1u << 1)

#define TIMER_STATUS_DONE    (1u << 0)

/* =========================================================
   API Prototypes
   ========================================================= */
void timer_set_value(uint32_t value);
void timer_init(uint32_t mode);
void timer_start(void);
uint32_t timer_status(void);
void timer_getcount(uint32_t *count);
void timer_clear_done(void);
void timer_delay_us(uint32_t val);
void timer_delay_ms(uint32_t val);
#endif
