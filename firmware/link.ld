MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 64K
  RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 20K
}

SECTIONS
{
  .isr_vector : { *(.isr_vector) } > FLASH
  .text   : { *(.text*) }   > FLASH
  .rodata : { *(.rodata*) } > FLASH
  _sidata = LOADADDR(.data);
  .data : { _sdata = .; *(.data*) ; _edata = .; } > RAM AT> FLASH
  .bss  : { _sbss = .; *(.bss*) *(COMMON) ; _ebss = .; } > RAM
}
