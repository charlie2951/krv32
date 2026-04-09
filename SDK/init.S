# -------------------------------------------------
# Bootloader Startup Script (Modified for 8KB)
# -------------------------------------------------

.equ MEM_START, 0xD0000000
.equ MEM_SIZE,  0x2000       # 8KB (Changed from 0x800)

.section .text.start, "ax"  # Matches KEEP(*(.text.start)) in init.lds
.globl _start                # Standard entry point name
.type  _start, @function

_start:
    # Cleaning up registers
    li x1, 0
    li x2, 0
    li x3, 0
    li x4, 0
    li x5, 0
    li x6, 0
    li x7, 0
    li x8, 0
    li x9, 0
    li x10, 0
    li x11, 0
    li x12, 0
    li x13, 0
    li x14, 0
    li x15, 0
    li x16, 0
    li x17, 0
    li x18, 0
    li x19, 0
    li x20, 0
    li x21, 0
    li x22, 0
    li x23, 0
    li x24, 0
    li x25, 0
    li x26, 0
    li x27, 0
    li x28, 0
    li x29, 0
    li x30, 0
    li x31, 0
    # Set Stack Pointer to the end of the 4KB block
    # Stack grows downwards in RISC-V (from 0xA0001000)
    li sp, (MEM_START + MEM_SIZE)
    
    # Call the C  function from boot_logic_app.c
    call hidden_bootloader_copy
    
    li t0, 0x00000000
    # Optional: Clear the 'ra' and 'gp' so the app can't "return" to the uploader
    li ra, 0
    li gp, 0
    jr t0  # Jump to the start of the app in BRAM
    # If main ever returns, trap the CPU
loop_forever:
    ebreak
    j loop_forever

