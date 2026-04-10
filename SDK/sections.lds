/*
 * Minimal GC-friendly linker script for RV32I
 */

ENTRY(_start)

MEMORY
{
    mem (rxw) : ORIGIN = 0x00000000, LENGTH = 64K
}

SECTIONS
{
    /* -------------------------------------------------
     * Text / Code section
     * ------------------------------------------------- */
    .text :
    {
        . = ALIGN(4);

        /* Keep startup / reset code */
        KEEP(*(.text.start))
        KEEP(*(.text.startup))
        KEEP(*(.init))

        /* All remaining code (GC will remove unused) */
        *(.text*)
        *(.rodata*)

        . = ALIGN(4);
    } > mem

    /* -------------------------------------------------
     * Initialized data
     * ------------------------------------------------- */
    .data :
    {
        . = ALIGN(4);
        *(.data*)
        . = ALIGN(4);
    } > mem

    /* -------------------------------------------------
     * Uninitialized data
     * ------------------------------------------------- */
    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
    } > mem

    /* -------------------------------------------------
     * End symbol
     * ------------------------------------------------- */
    _end = .;
    /* Define the stack pointer at the end of the 8K memory block */
    _stack_ptr = ORIGIN(mem) + LENGTH(mem);
}
