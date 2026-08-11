#include <stdint.h>

#define RCC_APB2ENR (*(volatile uint32_t *)0x40021018)
#define GPIOC_CRH   (*(volatile uint32_t *)0x40011004)
#define GPIOC_BSRR  (*(volatile uint32_t *)0x40011010)

static void delay(volatile uint32_t n) {
    while (n--) __asm__("nop");
}

int main(void) {
    RCC_APB2ENR |= (1 << 4);    // IOPCEN: enable GPIOC clock
    GPIOC_CRH &= ~(0xF << 20);  // clear PC13 config bits
    GPIOC_CRH |= (0x1 << 20);   // PC13: output, 10MHz, push-pull

    for (;;) {
        GPIOC_BSRR = (1 << 29);  // reset PC13 -> LED on (active-low)
        delay(500000);
        GPIOC_BSRR = (1 << 13);  // set PC13 -> LED off
        delay(500000);
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
