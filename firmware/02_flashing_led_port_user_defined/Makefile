CC = arm-none-eabi-gcc
CFLAGS_COMMON  = -mcpu=cortex-m3 -mthumb -Wall -ffreestanding -nostdlib -T ../link.ld
CFLAGS_RELEASE = $(CFLAGS_COMMON) -O2
CFLAGS_DEBUG   = $(CFLAGS_COMMON) -O0 -g

blink.elf: main.c ../link.ld
	$(CC) $(CFLAGS_RELEASE) -o $@ main.c

debug: main.c ../link.ld
	$(CC) $(CFLAGS_DEBUG) -o blink.elf main.c

flash: blink.elf
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program blink.elf verify reset exit"

clean:
	rm -f blink.elf

.PHONY: flash clean debug
