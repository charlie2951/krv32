/* -------------------------------------------------
 * RV32I Bootloader Startup Code
 * Lives in ROM at 0xD0000000
 * ------------------------------------------------- */

    .section .text.start, "ax"
    .globl _start
    .type  _start, @function

_start:
    /* 1. Register Cleanup */
    # (Keeping your cleanup logic is good practice)
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

    /* 2. Initialize Stack Pointer */
    # Use the linker symbol _stack_ptr which points to the top of BRAM
    la sp, _stack_ptr

    /* 3. Call the main function Logic */
    # This is the C function that copies Flash -> BRAM (0x00000000)
    call main

    
    /* 5. Halt (In case the app returns) */
halt:
    ebreak
    j halt