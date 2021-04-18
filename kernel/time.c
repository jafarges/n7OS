#include <n7OS/time.h>
#include <inttypes.h>
#include <n7OS/cpu.h>
#include <n7OS/irq.h>
#include <stdio.h>

#define F_OSC 0x1234BD

extern void handler_timer();

void init_timer(){
    // Set command : Impulsion generator / Binary encoded frequency
    outb(0x34,0x43);

    // Set frequency for channel 0
    uint16_t target_clk = 1000;
    uint16_t f_ch0 = F_OSC/target_clk;
    outb(f_ch0 & 0xFF,0x40);
    outb(f_ch0>>8,0x40);

    // Activating the timer interruption
    outb(inb(0x21)|0xFE,0x21);

    // Binding timer interruption to its handler
    init_irq_entry(0x20,(uint32_t)&handler_timer);
}

void __handler_timer(){
    // ACK
    outb(0x20,0x20);
    printf("A\n");
}