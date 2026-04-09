#include "qspi.h"
#include "printf.h"
// -----------------------------
static inline void wait_idle(void) {
    int timeout = 1000000;
    while ((QSPI_REG_STATUS & QSPI_STATUS_BUSY) && timeout--)
        ;

    if (timeout <= 0) {
        printf("ERROR: QSPI stuck busy!\n");
    }
}

static inline void wait_done(void) {
    while (!(QSPI_REG_STATUS & QSPI_STATUS_DONE));
}

// -----------------------------
static void qspi_exec(uint8_t cmd, uint32_t addr, uint32_t data,
                      uint8_t write, uint8_t dummy,
                      uint8_t quad_addr, uint8_t burst)
{
    wait_idle();

    QSPI_REG_ADDR = addr;
    QSPI_REG_DATA = data;

    uint32_t ctrl = QSPI_CTRL_START |
                    (write ? QSPI_CTRL_MODE_WR : QSPI_CTRL_MODE_RD) |
                    (quad_addr ? QSPI_CTRL_QUAD_ADDR : 0) |
                    (burst ? QSPI_CTRL_BURST_EN : 0) |
                    QSPI_CTRL_DUMMY(dummy) |
                    QSPI_CTRL_CMD(cmd);

    QSPI_REG_CTRL = ctrl;

    wait_done();
}

// -----------------------------
void qspi_init(void) {
    wait_idle();
}

// -----------------------------
uint32_t qspi_read_id(void) {
    qspi_exec(FLASH_CMD_READ_ID, 0, 0, 0, 0, 0, 0);
    return QSPI_REG_DATA;
}

// -----------------------------
static uint8_t read_sr(void) {
    qspi_exec(FLASH_CMD_RDSR, 0, 0, 0, 0, 0, 0);
    return (uint8_t)QSPI_REG_DATA;
}

// -----------------------------
void qspi_wait_flash_ready(void) {
    while (read_sr() & FLASH_SR_WIP);
}

// -----------------------------
static void write_sr(uint8_t sr) {
    qspi_exec(FLASH_CMD_WREN, 0, 0, 1, 0, 0, 0);
    qspi_exec(FLASH_CMD_WRSR, 0, sr, 1, 0, 0, 0);
    qspi_wait_flash_ready();
}

// -----------------------------
void qspi_flash_init(void) {
    uint32_t id = qspi_read_id();
    uint8_t manuf = id & 0xFF;

    uint8_t sr = read_sr();

    // Winbond
    if (manuf == 0xEF) {
        qspi_exec(FLASH_CMD_RDSR2, 0, 0, 0, 0, 0, 0);
        uint8_t sr2 = (uint8_t)QSPI_REG_DATA;

        if (!(sr2 & (1 << 1))) {
            qspi_exec(FLASH_CMD_WREN, 0, 0, 1, 0, 0, 0);
            sr2 |= (1 << 1);
            qspi_exec(FLASH_CMD_WRSR2, 0, sr2, 1, 0, 0, 0);
            qspi_wait_flash_ready();
        }
    }

    // Macronix
    else if (manuf == 0xC2) {
        if (!(sr & FLASH_SR_QE)) {
            sr |= FLASH_SR_QE;
            write_sr(sr);
        }
    }
}

// -----------------------------
void qspi_erase_sector(uint32_t addr) {
    qspi_exec(FLASH_CMD_WREN, 0, 0, 1, 0, 0, 0);
    qspi_exec(FLASH_CMD_SECTOR_ER, addr, 0, 1, 0, 0, 0);
    qspi_wait_flash_ready();
}

// -----------------------------
void qspi_write_word(uint32_t addr, uint32_t data) {
    qspi_exec(FLASH_CMD_WREN, 0, 0, 1, 0, 0, 0);
    qspi_exec(FLASH_CMD_PAGE_PROG, addr, data, 1, 0, 0, 0);
    qspi_wait_flash_ready();
}

// -----------------------------
static void page_program(uint32_t addr, const uint8_t *buf, uint32_t len) {

    for (uint32_t i = 0; i < len; i += 4) {

        uint32_t data = 0;

        for (int j = 0; j < 4 && (i + j) < len; j++) {
            data |= ((uint32_t)buf[i + j]) << (8 * j);
        }

        qspi_write_word(addr + i, data);
    }
}

// -----------------------------
void qspi_write_buffer(uint32_t addr, const uint8_t *buf, uint32_t len) {

    while (len > 0) {

        uint32_t offset = addr % FLASH_PAGE_SIZE;
        uint32_t space  = FLASH_PAGE_SIZE - offset;

        uint32_t chunk = (len < space) ? len : space;

        page_program(addr, buf, chunk);

        addr += chunk;
        buf  += chunk;
        len  -= chunk;
    }
}

// -----------------------------
uint32_t qspi_read_word_quad(uint32_t addr) {
    qspi_exec(FLASH_CMD_QUAD_READ, addr, 0, 0, 8, 0, 0);
    return QSPI_REG_DATA;
}

// -----------------------------
uint32_t qspi_read_word_quad_io(uint32_t addr) {
    qspi_exec(FLASH_CMD_QUAD_IO_RD, addr, 0, 0, 6, 1, 0);
    return QSPI_REG_DATA;
}