/*
Application boot code 
It copy Flash content to BRAM
Then start execution of main Application
Copyright: Subir Maity
*/
#include <stdint.h>
#include "mx25.h"
#include "printf.h"
#include "gpio.h"

#define FLASH_APP_START 0x3FE000  // Last 8KB of 4MB Flash
#define BRAM_START      0x00000000
#define APP_SIZE        8192
#define PAGE_SIZE       32

 uint8_t page_buf[PAGE_SIZE];

void hidden_bootloader_copy(void) {
   
    uint32_t *dest = (uint32_t *)BRAM_START;
     uint16_t id;
     int retry;
     pinMode(8,OUTPUT);
     digitalWrite(8,HIGH);//make pin high
     printf("\r\nKRV-32 Running on Basys-3...");
     printf("\r\nInitializing Flash....");
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
   printf("Flash Ready. ID: %0.4x\n\r", id);
    printf("Copying Flash content to BRAM.... \r\n");

    for (uint32_t addr = FLASH_APP_START; addr < (FLASH_APP_START + APP_SIZE); addr += PAGE_SIZE) {
        mx25_read(addr, page_buf, PAGE_SIZE); //reading 256 pages at a time
        
        // Copy to BRAM with Little Endian conversion
        for (int i = 0; i < PAGE_SIZE; i += 4) {
            *dest++ = ((uint32_t)page_buf[i+3] << 24) |
                      ((uint32_t)page_buf[i+2] << 16) |
                      ((uint32_t)page_buf[i+1] << 8)  |
                      ((uint32_t)page_buf[i]);
        }
    }
    printf("\r\nCopy completed....Executing your code...\r\n");
    digitalWrite(8,LOW);//make pin high
    /*
    // Force the compiler to jump to 0x0 using RISC-V assembly
    __asm__ volatile (
        "li a0, 0x00000000\n\t"  // Load immediate 0 into register a0
        "jr a0\n\t"              // Jump to register a0 (PC = 0)
    );
    */
}

    