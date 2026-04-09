#include <stdint.h>
#include "printf.h"
#include "mx25.h"

// 4MB Flash Boundaries
#define FLASH_SIZE_BYTES     0x400000  // 4,194,304 Bytes
#define LAST_SECTOR_ADDR     0x3FF000  // Start of the last 4KB sector
#define LAST_PAGE_ADDR       0x3FFF00  // Start of the last 256B page

int main(void) {
    uint8_t write_buf[256];
    uint8_t read_buf[256];
    uint16_t id;

    printf("--- Macronix 4MB Flash Debug (Last Sector Test) ---\n");

    // 1. Init
    if (!mx25_init(10)) {
        id = mx25_get_id();
        printf("Fail: Flash not found. ID Recv: %x\n", id);
        return -1;
    }
    printf("Flash Ready. ID: %x\n", mx25_get_id());

    // 2. Erase Last Sector (4KB)
    printf("Erasing Last Sector at %x...", LAST_SECTOR_ADDR);
    mx25_erase_sector(LAST_SECTOR_ADDR);
    printf(" Done.\n");

    // 3. Prepare Data (Pattern: 0xAA, 0x55...)
    for (int i = 0; i < 256; i++) {
        write_buf[i] = (i % 2 == 0) ? 0xAA : 0x55;
        read_buf[i] = 0x00;
    }

    // 4. Write to Last Page (Absolute end of memory)
    printf("Writing to Last Page at %x...", LAST_PAGE_ADDR);
    mx25_write_page(LAST_PAGE_ADDR, write_buf, 256);
    printf(" Done.\n");

    // 5. Read Back and Verify
    printf("Verifying Data...");
    mx25_read(LAST_PAGE_ADDR, read_buf, 256);

    int errors = 0;
    for (int i = 0; i < 256; i++) {
        if (read_buf[i] != write_buf[i]) {
            errors++;
        }
    }

    if (errors == 0) {
        printf(" PASS! Absolute last byte (0x3FFFFF) verified.\n");
    } else {
        printf(" FAIL! Found %d mismatches.\n", errors);
    }

    return 0;
}