SECTIONS
{
    . = 0x80000;
    .text : { 
        KEEP(*(.text.boot)) 
        *(.text) 
    }
    .rodata : { *(.rodata) }
    .data : { *(.data) }
    .bss (NOLOAD) : {
        . = ALIGN(16);
        __bss_start = .;
        *(.bss)
        __bss_end = .;
    }
    _stack_top = 0x200000;
    _buffer_start = 0x400000;
}