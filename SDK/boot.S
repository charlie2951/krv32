# -------------------------------------------------
# Bootloader Startup Script (Modified for 4KB)
# -------------------------------------------------

.equ MEM_START, 0xA0000000
.equ MEM_SIZE,  0x2000       # 4KB (Changed from 0x800)

.section .text.start         # Matches KEEP(*(.text.start)) in boot.lds
.globl _start                # Standard entry point name

_start:
    # Set Stack Pointer to the end of the 4KB block
    # Stack grows downwards in RISC-V (from 0xA0001000)
    li sp, (MEM_START + MEM_SIZE)

    # Call the C 'main' function from bootloader.c
    call main

    # If main ever returns, trap the CPU
loop_forever:
    ebreak
    j loop_forever