#include <n7OS/irq.h>
#include <n7OS/cpu.h>
#include <stdio.h>

extern void handler_IT();

/*
void handler_en_C() {
    printf("Interruption 50 reçu!\n");
}

void init_irq() {
    init_irq_entry(50,(uint32_t)&handler_en_C);
    sti();
}

*/