#include <printf.h>
#include "qspi.h"

#define TEST_ADDR  0x80000000
#define TEST_WORD  0xDEADBEEF

int main() {

    printf("\n==== QSPI TEST START ====\n");
printf("STATUS = %x\n", QSPI_REG_STATUS);
    qspi_init();
    qspi_flash_init();

    uint32_t id = qspi_read_id();
    printf("JEDEC ID: %x\n", id);

    if ((id & 0xFF) == 0x00 || (id & 0xFF) == 0xFF) {
        printf("ERROR: Flash not detected\n");
        return -1;
    }

    // -------------------------
    // ERASE
    // -------------------------
    printf("Erasing sector...\n");
    qspi_erase_sector(TEST_ADDR);

    uint32_t val = qspi_read_word_quad(TEST_ADDR);
    if (val != 0xFFFFFFFF) {
        printf("Erase failed: %x\n", val);
        return -1;
    }

    // -------------------------
    // WRITE
    // -------------------------
    printf("Writing word...\n");
    qspi_write_word(TEST_ADDR, TEST_WORD);

    // -------------------------
    // READ 0x6B
    // -------------------------
    val = qspi_read_word_quad(TEST_ADDR);
    printf("Read (0x6B): %x\n", val);

    if (val != TEST_WORD) {
        printf("FAIL (quad read)\n");
        return -1;
    }

    // -------------------------
    // READ 0xEB
    // -------------------------
    val = qspi_read_word_quad_io(TEST_ADDR);
    printf("Read (0xEB): %x\n", val);

    if (val != TEST_WORD) {
        printf("FAIL (quad IO read)\n");
        return -1;
    }

    // -------------------------
    // BUFFER WRITE TEST
    // -------------------------
    uint8_t buf[4] = {13,14,15,16};

    printf("Buffer write test...\n");
    qspi_write_buffer(TEST_ADDR + 0x100, buf, 4);

    for (int i = 0; i < 4; i++) {
        uint32_t r = qspi_read_word_quad(TEST_ADDR + 0x100 + i*4);
        printf("BUF[%d] = %x\n", i, r);
    }

    printf("\n==== QSPI TEST PASSED ====\n");

    while (1);
    return 0;
}