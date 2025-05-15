// Created with Corsair v1.0.4
#ifndef __REGS_PWM_H
#define __REGS_PWM_H

#define __I  volatile const // 'read only' permissions
#define __O  volatile       // 'write only' permissions
#define __IO volatile       // 'read / write' permissions


#ifdef __cplusplus
#include <cstdint>
extern "C" {
#else
#include <stdint.h>
#endif

#define PWM_BASE_ADDR 0x40000000

// CHANNEL_SEL0 - PWM Channel select register-0
#define PWM_CHANNEL_SEL0_ADDR 0x0
#define PWM_CHANNEL_SEL0_RESET 0x0
typedef struct {
    uint32_t CH0 : 1; // PWM Channel-0 select
    uint32_t : 31; // reserved
} pwm_channel_sel0_t;

// CHANNEL_SEL0.CH0 - PWM Channel-0 select
#define PWM_CHANNEL_SEL0_CH0_WIDTH 1
#define PWM_CHANNEL_SEL0_CH0_LSB 0
#define PWM_CHANNEL_SEL0_CH0_MASK 0x1
#define PWM_CHANNEL_SEL0_CH0_RESET 0x0

// CHANNEL_SEL1 - PWM Channel select register-1
#define PWM_CHANNEL_SEL1_ADDR 0x4
#define PWM_CHANNEL_SEL1_RESET 0x0
typedef struct {
    uint32_t CH1 : 1; // PWM Channel-1 select
    uint32_t : 31; // reserved
} pwm_channel_sel1_t;

// CHANNEL_SEL1.CH1 - PWM Channel-1 select
#define PWM_CHANNEL_SEL1_CH1_WIDTH 1
#define PWM_CHANNEL_SEL1_CH1_LSB 0
#define PWM_CHANNEL_SEL1_CH1_MASK 0x1
#define PWM_CHANNEL_SEL1_CH1_RESET 0x0

// CHANNEL_SEL2 - PWM Channel select register-2
#define PWM_CHANNEL_SEL2_ADDR 0x8
#define PWM_CHANNEL_SEL2_RESET 0x0
typedef struct {
    uint32_t CH2 : 1; // PWM Channel-2 select
    uint32_t : 31; // reserved
} pwm_channel_sel2_t;

// CHANNEL_SEL2.CH2 - PWM Channel-2 select
#define PWM_CHANNEL_SEL2_CH2_WIDTH 1
#define PWM_CHANNEL_SEL2_CH2_LSB 0
#define PWM_CHANNEL_SEL2_CH2_MASK 0x1
#define PWM_CHANNEL_SEL2_CH2_RESET 0x0

// CHANNEL_SEL3 - PWM Channel select register-3
#define PWM_CHANNEL_SEL3_ADDR 0xc
#define PWM_CHANNEL_SEL3_RESET 0x0
typedef struct {
    uint32_t CH3 : 1; // PWM Channel-3 select
    uint32_t : 31; // reserved
} pwm_channel_sel3_t;

// CHANNEL_SEL3.CH3 - PWM Channel-3 select
#define PWM_CHANNEL_SEL3_CH3_WIDTH 1
#define PWM_CHANNEL_SEL3_CH3_LSB 0
#define PWM_CHANNEL_SEL3_CH3_MASK 0x1
#define PWM_CHANNEL_SEL3_CH3_RESET 0x0

// DUTY_CYCLE0 - Duty Cycle select Register-0
#define PWM_DUTY_CYCLE0_ADDR 0x10
#define PWM_DUTY_CYCLE0_RESET 0x0
typedef struct {
    uint32_t CH0 : 8; // CH0 Duty cycle 8 bit value 0-255
    uint32_t : 24; // reserved
} pwm_duty_cycle0_t;

// DUTY_CYCLE0.CH0 - CH0 Duty cycle 8 bit value 0-255
#define PWM_DUTY_CYCLE0_CH0_WIDTH 8
#define PWM_DUTY_CYCLE0_CH0_LSB 0
#define PWM_DUTY_CYCLE0_CH0_MASK 0xff
#define PWM_DUTY_CYCLE0_CH0_RESET 0x0

// DUTY_CYCLE1 - Duty Cycle select Register-1
#define PWM_DUTY_CYCLE1_ADDR 0x14
#define PWM_DUTY_CYCLE1_RESET 0x0
typedef struct {
    uint32_t CH1 : 8; // CH1 Duty cycle 8 bit value 0-255
    uint32_t : 24; // reserved
} pwm_duty_cycle1_t;

// DUTY_CYCLE1.CH1 - CH1 Duty cycle 8 bit value 0-255
#define PWM_DUTY_CYCLE1_CH1_WIDTH 8
#define PWM_DUTY_CYCLE1_CH1_LSB 0
#define PWM_DUTY_CYCLE1_CH1_MASK 0xff
#define PWM_DUTY_CYCLE1_CH1_RESET 0x0

// DUTY_CYCLE2 - Duty Cycle select Register-2
#define PWM_DUTY_CYCLE2_ADDR 0x18
#define PWM_DUTY_CYCLE2_RESET 0x0
typedef struct {
    uint32_t CH2 : 8; // CH2 Duty cycle 8 bit value 0-255
    uint32_t : 24; // reserved
} pwm_duty_cycle2_t;

// DUTY_CYCLE2.CH2 - CH2 Duty cycle 8 bit value 0-255
#define PWM_DUTY_CYCLE2_CH2_WIDTH 8
#define PWM_DUTY_CYCLE2_CH2_LSB 0
#define PWM_DUTY_CYCLE2_CH2_MASK 0xff
#define PWM_DUTY_CYCLE2_CH2_RESET 0x0

// DUTY_CYCLE3 - Duty Cycle select Register-3
#define PWM_DUTY_CYCLE3_ADDR 0x1c
#define PWM_DUTY_CYCLE3_RESET 0x0
typedef struct {
    uint32_t CH3 : 8; // CH3 Duty cycle 8 bit value 0-255
    uint32_t : 24; // reserved
} pwm_duty_cycle3_t;

// DUTY_CYCLE3.CH3 - CH3 Duty cycle 8 bit value 0-255
#define PWM_DUTY_CYCLE3_CH3_WIDTH 8
#define PWM_DUTY_CYCLE3_CH3_LSB 0
#define PWM_DUTY_CYCLE3_CH3_MASK 0xff
#define PWM_DUTY_CYCLE3_CH3_RESET 0x0

// PWM_OUT0 - PWM Output register-0
#define PWM_PWM_OUT0_ADDR 0x20
#define PWM_PWM_OUT0_RESET 0x0
typedef struct {
    uint32_t PWM0 : 1; // Channel-0 PWM output
    uint32_t : 31; // reserved
} pwm_pwm_out0_t;

// PWM_OUT0.PWM0 - Channel-0 PWM output
#define PWM_PWM_OUT0_PWM0_WIDTH 1
#define PWM_PWM_OUT0_PWM0_LSB 0
#define PWM_PWM_OUT0_PWM0_MASK 0x1
#define PWM_PWM_OUT0_PWM0_RESET 0x0

// PWM_OUT1 - PWM Output register-1
#define PWM_PWM_OUT1_ADDR 0x24
#define PWM_PWM_OUT1_RESET 0x0
typedef struct {
    uint32_t PWM1 : 1; // Channel-1 PWM output
    uint32_t : 31; // reserved
} pwm_pwm_out1_t;

// PWM_OUT1.PWM1 - Channel-1 PWM output
#define PWM_PWM_OUT1_PWM1_WIDTH 1
#define PWM_PWM_OUT1_PWM1_LSB 0
#define PWM_PWM_OUT1_PWM1_MASK 0x1
#define PWM_PWM_OUT1_PWM1_RESET 0x0

// PWM_OUT2 - PWM Output register-2
#define PWM_PWM_OUT2_ADDR 0x28
#define PWM_PWM_OUT2_RESET 0x0
typedef struct {
    uint32_t PWM2 : 1; // Channel-2 PWM output
    uint32_t : 31; // reserved
} pwm_pwm_out2_t;

// PWM_OUT2.PWM2 - Channel-2 PWM output
#define PWM_PWM_OUT2_PWM2_WIDTH 1
#define PWM_PWM_OUT2_PWM2_LSB 0
#define PWM_PWM_OUT2_PWM2_MASK 0x1
#define PWM_PWM_OUT2_PWM2_RESET 0x0

// PWM_OUT3 - PWM Output register-3
#define PWM_PWM_OUT3_ADDR 0x2c
#define PWM_PWM_OUT3_RESET 0x0
typedef struct {
    uint32_t PWM3 : 1; // Channel-3 PWM output
    uint32_t : 31; // reserved
} pwm_pwm_out3_t;

// PWM_OUT3.PWM3 - Channel-3 PWM output
#define PWM_PWM_OUT3_PWM3_WIDTH 1
#define PWM_PWM_OUT3_PWM3_LSB 0
#define PWM_PWM_OUT3_PWM3_MASK 0x1
#define PWM_PWM_OUT3_PWM3_RESET 0x0


// Register map structure
typedef struct {
    union {
        __O uint32_t CHANNEL_SEL0; // PWM Channel select register-0
        __O pwm_channel_sel0_t CHANNEL_SEL0_bf; // Bit access for CHANNEL_SEL0 register
    };
    union {
        __O uint32_t CHANNEL_SEL1; // PWM Channel select register-1
        __O pwm_channel_sel1_t CHANNEL_SEL1_bf; // Bit access for CHANNEL_SEL1 register
    };
    union {
        __O uint32_t CHANNEL_SEL2; // PWM Channel select register-2
        __O pwm_channel_sel2_t CHANNEL_SEL2_bf; // Bit access for CHANNEL_SEL2 register
    };
    union {
        __O uint32_t CHANNEL_SEL3; // PWM Channel select register-3
        __O pwm_channel_sel3_t CHANNEL_SEL3_bf; // Bit access for CHANNEL_SEL3 register
    };
    union {
        __O uint32_t DUTY_CYCLE0; // Duty Cycle select Register-0
        __O pwm_duty_cycle0_t DUTY_CYCLE0_bf; // Bit access for DUTY_CYCLE0 register
    };
    union {
        __O uint32_t DUTY_CYCLE1; // Duty Cycle select Register-1
        __O pwm_duty_cycle1_t DUTY_CYCLE1_bf; // Bit access for DUTY_CYCLE1 register
    };
    union {
        __O uint32_t DUTY_CYCLE2; // Duty Cycle select Register-2
        __O pwm_duty_cycle2_t DUTY_CYCLE2_bf; // Bit access for DUTY_CYCLE2 register
    };
    union {
        __O uint32_t DUTY_CYCLE3; // Duty Cycle select Register-3
        __O pwm_duty_cycle3_t DUTY_CYCLE3_bf; // Bit access for DUTY_CYCLE3 register
    };
    union {
        __I uint32_t PWM_OUT0; // PWM Output register-0
        __I pwm_pwm_out0_t PWM_OUT0_bf; // Bit access for PWM_OUT0 register
    };
    union {
        __I uint32_t PWM_OUT1; // PWM Output register-1
        __I pwm_pwm_out1_t PWM_OUT1_bf; // Bit access for PWM_OUT1 register
    };
    union {
        __I uint32_t PWM_OUT2; // PWM Output register-2
        __I pwm_pwm_out2_t PWM_OUT2_bf; // Bit access for PWM_OUT2 register
    };
    union {
        __I uint32_t PWM_OUT3; // PWM Output register-3
        __I pwm_pwm_out3_t PWM_OUT3_bf; // Bit access for PWM_OUT3 register
    };
} pwm_t;

#define PWM ((pwm_t*)(PWM_BASE_ADDR))

#ifdef __cplusplus
}
#endif

#endif /* __REGS_PWM_H */