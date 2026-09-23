/*
** This linker script is intended for use in breadboard deployments that
** include the Va416x0.GdsSupport component. The GdsStream and TlmGdsChan
** components which are part of the GdsSupport topology require external SRAM
** allocations, but the breadboards do not have external memories, so this
** defines the .EXTI_SRAM section to be inside DATA_SRAM0 to compensate.
*/

SECTIONS
{
    /* External SRAM */
    /* We can only use the real EXTI_SRAM for BSS. This isn't a real external
       SRAM, but if we don't mark it as NOLOAD, then the ELF file will
       indicate a LOAD segment for this fake EXTI_SRAM, just like it would for
       the real EXTI_SRAM, and then the .bin file would be padded out to cover
       the segment. By marking this as NOLOAD, we can avoid inflating our .bin
       files to 0.5 GiB and keep them at most 256 KiB. */
    .EXTI_SRAM (NOLOAD) : ALIGN(2)
    {
        __extsram_data_start = .;
        KEEP(*(.EXTI_SRAM))
        __extsram_data_end = .;
    } > DATA_SRAM0
}
