#include <stdint.h>

// Board wiring, not in RM0008/datasheet: Blue Pill schematic hardwires the
// onboard LED to PC13 (active-low). Any other port/pin toggles correctly
// but drives nothing visible unless you wire an external LED to it.
#define LED_PORT 'C'  // change this: A-G
#define LED_PIN  13   // change this: 0-15

// RM0008 (STM32F1 reference manual, Rev 21) Table 3, p.50: GPIOA base
// 0x40010800, each port +0x400 (A,B,C,D,E,F,G = index 0..6).
#define PORT_INDEX (LED_PORT - 'A')
#define GPIO_BASE  (0x40010800u + PORT_INDEX * 0x400u)

// RM0008 7.3.7 RCC_APB2ENR, p.113: bit2=IOPAEN .. bit8=IOPGEN (port index + 2).
#define RCC_APB2ENR (*(volatile uint32_t *)0x40021018)
// RM0008 9.2.1 GPIOx_CRL (pins 0-7), p.171, offset 0x00.
#define GPIO_CRL (*(volatile uint32_t *)(GPIO_BASE + 0x00))
// RM0008 9.2.2 GPIOx_CRH (pins 8-15), p.172, offset 0x04.
#define GPIO_CRH (*(volatile uint32_t *)(GPIO_BASE + 0x04))
// RM0008 9.2.5 GPIOx_BSRR, p.173, offset 0x10: bits 0-15 set, 16-31 reset.
#define GPIO_BSRR (*(volatile uint32_t *)(GPIO_BASE + 0x10))

#define DEFAULT_DELAY 1000000  // 1s

static void delay(volatile uint32_t n) {
    while (n--) __asm__("nop");
}

int main(void) {
    RCC_APB2ENR |= (1 << (PORT_INDEX + 2));  // IOPxEN

    // CNFy=00 (push-pull), MODEy=01 (10MHz) -> nibble 0x1, per Table 20
    // "Port bit configuration table", RM0008 p.171-172.
    if (LED_PIN < 8) {
        GPIO_CRL &= ~(0xF << (LED_PIN * 4));
        GPIO_CRL |= (0x1 << (LED_PIN * 4));  // output, 10MHz, push-pull
    } else {
        GPIO_CRH &= ~(0xF << ((LED_PIN - 8) * 4));
        GPIO_CRH |= (0x1 << ((LED_PIN - 8) * 4));
    }

    for (;;) {
        GPIO_BSRR = (1 << (LED_PIN + 16));  // reset -> LED on (active-low)
        delay(DEFAULT_DELAY);
        GPIO_BSRR = (1 << LED_PIN);  // set -> LED off
        delay(DEFAULT_DELAY);
    }
}

void reset_handler(void) {
    extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss;
    uint32_t       *src = &_sidata, *dst = &_sdata;
    while (dst < &_edata) *dst++ = *src++;
    dst = &_sbss;
    while (dst < &_ebss) *dst++ = 0;
    main();
    while (1);
}

__attribute__((section(".isr_vector"))) void (*const vector_table[])(void) = {
     (void (*)(void))0x20005000,  // initial SP: top of 20KB RAM
     reset_handler,
};
