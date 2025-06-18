ENTRY(_start)

SECTIONS {
    . = 0x00100000;

    .multiboot : {
        KEEP(*(.multiboot))
    }

    .trampoline 0x7C00 : {
        *(.trampoline)
    }

    .text : {
        *(.text)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    .bss : {
        *(COMMON)
        *(.bss)
    }

}