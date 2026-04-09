/*
KRV-32 Bootloader with Flash support
Version-3.0
Copyright: Subir Maity
Supported Device: 
-Basys-3 board with Macronix flash
*/
#include <stdint.h>
#include "delay.h"
#include "spi.h"
#include "mx25.h"
#include "seven_seg.h"
#include "printf.h"
#include "uart.h"
#include "gpio.h"

// 4MB Flash Boundaries
#define FLASH_SIZE_BYTES     0x400000  
#define EIGHT_KB             8192
#define PAGE_SIZE            32
#define SECTOR_SIZE          4096
// The start address for the last 8KB of Flash(0x400000 - 0x2000)
#define START_ADDR_8KB       (FLASH_SIZE_BYTES - EIGHT_KB)
//BRAM mem location//
#define BRAM_BASE   0x00000000
volatile uint32_t *bram = (uint32_t *)BRAM_BASE;

//helper function hex_to_u32()
uint32_t hex_to_u32(char *buf)
{
    uint32_t val = 0;
    for (int i = 0; i < 8; i++) {
        char c = buf[i];
        val <<= 4;
        if (c >= '0' && c <= '9')
            val |= (c - '0');
        else if (c >= 'a' && c <= 'f')
            val |= (c - 'a' + 10);
        else if (c >= 'A' && c <= 'F')
            val |= (c - 'A' + 10);
    }
    return val;
}

//print the BRAM stored data into Serial terminal
    void print_bram(void){
	int index=0; 
    printf("Printing BRAM content...\n\r");
//print BRAM content  
    while(bram[index]!=0xABCDEF99) //END OF BRAM DATA
    {
        printf("\r\nIndex: %0.4d, Data: %0.8x",index, bram[index]);
        index=index+1;
        delay(10000);
    }
}
//writing BRAM content to flash
// Modified writing BRAM content to flash
void write_flash(void) {
    uint8_t data_buf[PAGE_SIZE];
    uint8_t read_buf[PAGE_SIZE];
    uint16_t id;
    uint32_t bram_idx = 0; // Index for 32-bit BRAM access

    printf("\n--- Macronix 4MB Flash: Writing BRAM to Last 8KB ---\n\r");
   
    uint8_t retry;
    // 1. Init with Retry Loop 
    for (retry = 0; retry < 10; retry++) {
        if (mx25_init(10)) {
            id = mx25_get_id();
            if (id == 0xc215) break;
        }
    }

    if (id != 0xc215) {
        printf("Fail: Flash not found. ID Recv: %0.4x\n\r", id);
        return ;
    }


    printf("Flash Ready. ID: 0x%x\n\r", id);
    // 2. Erase Last 8KB
    printf("Erasing last 8KB...\n\r");
    for (uint32_t addr = START_ADDR_8KB; addr < FLASH_SIZE_BYTES; addr += SECTOR_SIZE) {
        mx25_erase_sector(addr);
    }
    printf("Erase Done.\n\r");

    // 3. Write BRAM to Flash (iterate thru no of pages total for 8KB)
    printf("Writing BRAM data to Flash...");
    for (uint32_t addr = START_ADDR_8KB; addr < FLASH_SIZE_BYTES; addr += PAGE_SIZE) {
        
        // Fill the n-byte page buffer from 32-bit BRAM
        for (int i = 0; i < PAGE_SIZE; i += 4) {
            uint32_t word = bram[bram_idx++];
            
            // Split 32-bit word into 4 bytes (Little Endian)
            data_buf[i]     = (uint8_t)(word & 0xFF);
            data_buf[i + 1] = (uint8_t)((word >> 8) & 0xFF);
            data_buf[i + 2] = (uint8_t)((word >> 16) & 0xFF);
            data_buf[i + 3] = (uint8_t)((word >> 24) & 0xFF);
        }
        
        mx25_write_page(addr, data_buf, PAGE_SIZE);
    }
    printf("\r\n Write Done.\n\r");

    // 4. Verify 8KB
    printf("Verifying...");
    int total_errors = 0;
    bram_idx = 0; // Reset index to check against start of BRAM

    for (uint32_t addr = START_ADDR_8KB; addr < FLASH_SIZE_BYTES; addr += PAGE_SIZE) {
        mx25_read(addr, read_buf, PAGE_SIZE);
        
        for (int i = 0; i < PAGE_SIZE; i += 4) {
            uint32_t expected_word = bram[bram_idx++];
            
            // Reconstruct word from read bytes to compare
            uint32_t read_word = (uint32_t)read_buf[i] |
                                 ((uint32_t)read_buf[i+1] << 8) |
                                 ((uint32_t)read_buf[i+2] << 16) |
                                 ((uint32_t)read_buf[i+3] << 24);

            if (read_word != expected_word) {
                total_errors++;
            }
        }
    }

    if (total_errors == 0) {
        printf(" PASS! BRAM successfully backed up to Flash.\n\r");
        ss_show_word(SS_WORD_DONE);
    } else {
        printf(" FAIL! Found %d word mismatches.\n\r", total_errors);
        ss_show_word(SS_WORD_FAIL);
    }
}

//---------- Main function -----------//
int main(void){
    volatile uint32_t address = 0;
    char hex_buf[8];
    volatile uint32_t idx = 0;
    const uint32_t MAX_WORDS = 2048; // Limit for 8KB Flash space
    
    // Configure GPIOs
    for (uint8_t pin = 8; pin <= 15; pin++)
        pinMode(pin, OUTPUT); // Configure GPIO8-15 (LEDs) as output

    ss_show_word(SS_WORD_BOOT); // Display BOOT message 
    digitalWritePort(8, 0xFF);  // All LEDs are ON
    
    printf("\n***KRV32 UART Bootloader with Flash support***\n\r");
    printf("Version-3.0 Copyright: Subir Maity\n\r");
    printf("Max capacity: 2048 words (8KB)\n\r");
    
    while(1)
    {
        char c = uart_receive(UART1);

        if (c == '\n' || c == '\r') {
            if (idx == 8) {
                uint32_t word = hex_to_u32(hex_buf);

                /* Check for End Marker */
                if (word == 0xFFFFFFFF) {
                    printf("\nTransfer DONE by marker. Writing to Flash...\n\r");
                    print_bram();
                    printf("\r\nLoading data to flash... \r\n");
                    write_flash();
                    //ss_show_word(SS_WORD_DONE);
                    digitalWritePort(8, 0x00); // All LEDs are OFF
                    while (1); // Stop here
                }

                /* Check for Memory Limit */
                if (address < MAX_WORDS) {
                    bram[address] = word;
                    address += 1;
                    
                    // Optional: Provide a small visual progress on LEDs every 256 words
                    /*
                    if (address % 256 == 0) {
                        printf("Received %d/2048 words...\n\r", address);
                    }
                        */
                } 
                else {
                    printf("\nERROR: BRAM Limit reached (8KB)! Forcing Flash Write...\n\r");
                   // write_flash();
                   // ss_show_word(SS_WORD_DONE);
                   // digitalWritePort(8, 0x00);
                    while (1); // Stop here to prevent memory corruption
                }
            }
            idx = 0;
        } 
        else {
            if (idx < 8)
                hex_buf[idx++] = c;
        }
    }
    return 0;
}