#include <stdint.h>
#include "printf.h"
#include "mx25.h"

// 4MB Flash Boundaries
#define FLASH_SIZE_BYTES     0x400000  // 4MB
#define READ_SIZE            0x2000    // 8KB (8192 Bytes)
#define START_ADDR           (FLASH_SIZE_BYTES - READ_SIZE) // 0x3FE000

int main(void) {
    uint8_t read_buf[256];
    uint16_t id = 0;
    int retry;

    printf("--- Macronix 4MB Flash: Read Last 8KB ---\n\r");

    // 1. Init with Retry Loop (as requested earlier)
    for (retry = 0; retry < 10; retry++) {
        if (mx25_init(10)) {
            id = mx25_get_id();
            if (id == 0xc215) break;
        }
    }

    if (id != 0xc215) {
        printf("Fail: Flash not found. ID Recv: %0.4x\n\r", id);
        return -1;
    }
    printf("Flash Ready. ID: %0.4x\n\r", id);

    // 2. Read and Print 8KB in 256-byte chunks
    printf("Reading from %0.8x to %0.8x (32-bit Little Endian)...\n\r", START_ADDR, FLASH_SIZE_BYTES - 1);

    for (uint32_t current_addr = START_ADDR; current_addr < FLASH_SIZE_BYTES; current_addr += 256) {
        // Read 256 bytes into the buffer
        mx25_read(current_addr, read_buf, 256);

        // Process the 256-byte buffer in 4-byte chunks (64 words per buffer)
        for (int i = 0; i < 256; i += 4) {
            // Concatenate 8-bit to 32-bit (Little Endian)
            // byte[i]   = LSB
            // byte[i+3] = MSB
            uint32_t data32 = ((uint32_t)read_buf[i+3] << 24) |
                              ((uint32_t)read_buf[i+2] << 16) |
                              ((uint32_t)read_buf[i+1] << 8)  |
                              ((uint32_t)read_buf[i]);

            // Print address and the concatenated 32-bit hex on a new line
            printf("Addr: %0.4d | Value: %0.8x\n\r", current_addr + i, data32);
        }
    }
    printf("\n\r--- Read Complete ---\n\r");
    return 0;
}