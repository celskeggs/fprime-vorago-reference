MEMORY
{
    /* External SRAM */
    EXTI_SRAM (rwx) : ORIGIN = 0x60040000, LENGTH = 1M
}

SECTIONS
{
    /* External SRAM sub-section used for code execution */
    .EXTI_SRAM.CODE : ALIGN(8)
    {
        __extsram_exec_start = .;
        KEEP(*(.EXTI_SRAM.CODE))
        __extsram_exec_end = .;
    } > EXTI_SRAM AT > CODE_SRAM
    /* Location in CODE_SRAM where the original code is located */
    __extsram_load_start = LOADADDR(.EXTI_SRAM.CODE);

    /* External SRAM */
    /* We can only use EXTI_SRAM for BSS. The VA41630's internal state machine
       can only load into the lowest 256 KiB of memory (ISRAM). If we don't
       mark this as NOLOAD, then the ELF file will indicate a LOAD segment for
       EXTI_SRAM, and then the .bin file will be padded out to cover the segment.
       So by marking this as NOLOAD, we can avoid inflating our .bin files to
       1.5 GiB and keep them at most 256 KiB. */
    .EXTI_SRAM (NOLOAD) : ALIGN(2)
    {
        /* Include code execution section of external SRAM */
        *(.EXTI_SRAM.CODE)

        __extsram_data_start = .;
        KEEP(*(.EXTI_SRAM))
        __extsram_data_end = .;
    } > EXTI_SRAM
    /* Starting address of the external SRAM memory region */
    __extsram_start = ORIGIN(EXTI_SRAM);
    /* Size of the external SRAM memory region */
    __extsram_size = LENGTH(EXTI_SRAM);
}
