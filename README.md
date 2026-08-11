# STM32 Blue Pill — Self-Study

Learning bare-metal embedded programming on the STM32F103C8T6 ("Blue Pill"
board), register-level, no HAL/framework. Arch Linux toolchain.

## Hardware

- STM32F103C8T6 "Blue Pill" board
- ST-Link V2 clone (SWD flashing/debugging)
- FT232RL FTDI USB-serial adapter (for UART)

## Toolchain

```bash
sudo pacman -S arm-none-eabi-gcc arm-none-eabi-newlib arm-none-eabi-binutils \
                arm-none-eabi-gdb openocd
```

VSCode + [Cortex-Debug](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug)
extension for source-level debugging (see `.vscode/launch.json`).

## Project structure

```
docs/          STM32F103C8 datasheet + RM0008 reference manual (register maps)
firmware/
  link.ld      Shared linker script (64K flash / 20K RAM) — one chip, one script
  01_.../      Each numbered dir is a standalone self-contained lesson:
  02_.../        main.c + Makefile, builds independently
```

## Build & flash

Inside any `firmware/NN_*/` dir:

```bash
make        # release build (-O2)
make flash  # flash to chip via openocd + ST-Link
make debug  # debug build (-O0 -g)
```

## Projects

1. **`01_flashing_led_port`** — PC13 onboard LED blink, hardcoded register
   addresses. First flash, register-level GPIO from scratch.
2. **`02_flashing_led_port_user_defined`** — same blink, but port/pin
   computed from `#define LED_PORT`/`LED_PIN` at the top of the file.
   Generalizes the register math (any port A-G, any pin 0-15).

## License

MIT — see [LICENSE](LICENSE).
