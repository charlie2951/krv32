// Created with Corsair v1.0.4
#ifndef __REGS_RX_H
#define __REGS_RX_H

#define __I  volatile const // 'read only' permissions
#define __O  volatile       // 'write only' permissions
#define __IO volatile       // 'read / write' permissions


#ifdef __cplusplus
#include <cstdint>
extern "C" {
#else
#include <stdint.h>
#endif

#define UART_RX_BASE_ADDR 0x30000000

// U_DATA - UART Data register
#define UART_RX_U_DATA_ADDR 0x0
#define UART_RX_U_DATA_RESET 0x0
typedef struct {
    uint32_t DATA : 8; // Data received Via UART RX
    uint32_t : 24; // reserved
} uart_rx_u_data_t;

// U_DATA.DATA - Data received Via UART RX
#define UART_RX_U_DATA_DATA_WIDTH 8
#define UART_RX_U_DATA_DATA_LSB 0
#define UART_RX_U_DATA_DATA_MASK 0xff
#define UART_RX_U_DATA_DATA_RESET 0x0

// U_STAT - UART Status register
#define UART_RX_U_STAT_ADDR 0x4
#define UART_RX_U_STAT_RESET 0x20
typedef struct {
    uint32_t : 5; // reserved
    uint32_t READY : 1; // UART is Ready to receive
    uint32_t : 7; // reserved
    uint32_t RX_DONE : 1; // Done Receiving 8-bit
    uint32_t : 18; // reserved
} uart_rx_u_stat_t;

// U_STAT.READY - UART is Ready to receive
#define UART_RX_U_STAT_READY_WIDTH 1
#define UART_RX_U_STAT_READY_LSB 5
#define UART_RX_U_STAT_READY_MASK 0x20
#define UART_RX_U_STAT_READY_RESET 0x1

// U_STAT.RX_DONE - Done Receiving 8-bit
#define UART_RX_U_STAT_RX_DONE_WIDTH 1
#define UART_RX_U_STAT_RX_DONE_LSB 13
#define UART_RX_U_STAT_RX_DONE_MASK 0x2000
#define UART_RX_U_STAT_RX_DONE_RESET 0x0

// U_CTRL - UART Control register
#define UART_RX_U_CTRL_ADDR 0x8
#define UART_RX_U_CTRL_RESET 0x0
typedef struct {
    uint32_t : 9; // reserved
    uint32_t START : 1; // RX Begin Signal, Valid For Only One Cycle
    uint32_t : 22; // reserved
} uart_rx_u_ctrl_t;

// U_CTRL.START - RX Begin Signal, Valid For Only One Cycle
#define UART_RX_U_CTRL_START_WIDTH 1
#define UART_RX_U_CTRL_START_LSB 9
#define UART_RX_U_CTRL_START_MASK 0x200
#define UART_RX_U_CTRL_START_RESET 0x0


// Register map structure
typedef struct {
    union {
        __IO uint32_t U_DATA; // UART Data register
        __IO uart_rx_u_data_t U_DATA_bf; // Bit access for U_DATA register
    };
    union {
        __IO uint32_t U_STAT; // UART Status register
        __IO uart_rx_u_stat_t U_STAT_bf; // Bit access for U_STAT register
    };
    union {
        __O uint32_t U_CTRL; // UART Control register
        __O uart_rx_u_ctrl_t U_CTRL_bf; // Bit access for U_CTRL register
    };
} uart_rx_t;

#define UART_RX ((uart_rx_t*)(UART_RX_BASE_ADDR))

#ifdef __cplusplus
}
#endif

#endif /* __REGS_RX_H */