SECTIONS
{
    .text 0x1078 :
    {
        *(.text.startup)
        *(.text)
        *(.data .data*)
        *(.rodata .rodata*)
        *(textend)
    }
}

