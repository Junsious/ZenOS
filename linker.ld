ENTRY(start)

SECTIONS
{
  . = 0x100000;  /* Адрес начала загрузки */

  .boot :
  {
    *(.multiboot_header)
  }

  .text :
  {
    *(.text)
  }

  .rodata :
  {
    *(.rodata)
  }

  .data :
  {
    *(.data)
  }

  .bss :
  {
    *(.bss)
  }
}
