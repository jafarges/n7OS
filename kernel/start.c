#include <n7OS/cpu.h>
#include <n7OS/irq.h>
#include <stdio.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>

extern void handler_IT();


void handler_en_C() {
    printf("Interruption 50 recu!\n");
}

void init_irq() {
    init_irq_entry(50,(uint32_t)&handler_IT);
    sti();
}

void kernel_start(void)
{
    
    // on ne doit jamais sortir de kernel_start
    while (1) {
        init_irq();
        printf("\fBienvenu sur Minishell!\nVersion O.1\n");
        
        __asm__ ("int $50");

        // cette fonction arrete le processeur
        hlt();
    }
}
