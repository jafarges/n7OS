#include <n7OS/cpu.h>
#include <n7OS/irq.h>
#include <stdio.h>
#include <n7OS/time.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <unistd.h>
#include <n7OS/sys.h>

extern void handler_IT();

void handler_en_C() {
    printf("Interruption 50 recu!\n");
}

void init_irq() {
    init_irq_entry(50,(uint32_t)&handler_IT);
    
}

void kernel_start(void)
{
    // Prepare timer
    init_timer();

    init_syscall();

    // Enable interruptions
    sti();

    // Welcoming screen
    printf("\f");
    printf("Bienvenu sur Minishell!\nVersion O.1\n");
    printf("\n");

    //shutdown(1);
    while (1) {
        // Show timer
        display_time();    
          
        //__asm__ ("int $50");
        
        // cette fonction arrete le processeur
        hlt();
    }
}
