OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY
{
    ROM : ORIGIN = 0x0       , LENGTH = 0x1000
    RAM : ORIGIN = 0x80000000, LENGTH = 0x10000
}

SECTIONS
{
    .text : ALIGN(4)
    {
        KEEP(*(.text.start*))
        *(.text)
        *(.text.*)
    } > ROM

    .rodata : ALIGN(4)
    {
        *(.rodata)
        *(.rodata.*)
    } > ROM

    . = ORIGIN(RAM);
    .data : ALIGN(4)
    {
        PROVIDE(_sidata = LOADADDR(.data));
        PROVIDE(_sdata = .);
        PROVIDE(__global_pointer$ = _sdata + 0x800);
        *(.data)
        *(.data.*)
        PROVIDE(_edata = .);
    } > RAM AT> ROM /* LMA in ROM, while VMA in RAM */

    .bss (NOLOAD) : ALIGN(4)
    {
        PROVIDE(_sbss = .);
        *(.bss)
        *(.bss.*)
        PROVIDE(_ebss = .);
    } >RAM AT> RAM

    __heap_size = 0x1000;
    .heap (NOLOAD) : ALIGN(4)
    {
        PROVIDE(_sheap = .);
        /* . += ABSOLUTE(__heap_size); This statement causes to section overflowed */
        . += __heap_size;
        PROVIDE(_eheap = .);
    } >RAM AT> RAM

    __stack_size = 0x2000;
    PROVIDE(__stack_size = __stack_size);
    .stack (NOLOAD) : ALIGN(4)
    {
        PROVIDE(_sstack = ORIGIN(RAM) + LENGTH(RAM));
        PROVIDE(__stack_top = _sstack);
        PROVIDE(_estack = _sstack - __stack_size);
        ASSERT(_estack >= _eheap, "Stack and Heap are overlapped!");
        . += __stack_size;
        PROVIDE(__stack_top = .);
    } >RAM AT> RAM

    /DISCARD/ :
    {
        *(.eh_frame)
        *(.comment)
        *(.debug*)
    }
}
