/*
 * Revised bootloader linker script
 * Optimized for GC-sections and matching standard structure
 */

ENTRY(_start)

MEMORY
{
    /* Bootloader specific memory region */
    mem (rxw) : ORIGIN = 0xD0000000, LENGTH = 8K
}

SECTIONS
{
    /* -------------------------------------------------
     * Code and Read-Only Data
     * ------------------------------------------------- */
    .text :
    {
        . = ALIGN(4);
        
        /* Ensure the init.s code stays at the very front */
        KEEP(*(.text.start))
        KEEP(*(.text.startup))
        KEEP(*(.init))
        
        /* All other code from source_code.c and drivers */
        *(.text*)
        *(.rodata*)
        
        . = ALIGN(4);
    } > mem

    /* -------------------------------------------------
     * Initialized Data
     * ------------------------------------------------- */
    .data :
    {
        . = ALIGN(4);
        *(.data*)
        . = ALIGN(4);
    } > mem

    /* -------------------------------------------------
     * Uninitialized Data (BSS)
     * ------------------------------------------------- */
    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
    } > mem

    /* Define end symbol for stack/heap tracking if needed */
    _end = .;
    end = .;
}