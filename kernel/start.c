#include <n7OS/cpu.h>
#include <n7OS/irq.h>
#include <stdio.h>
#include <n7OS/time.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <unistd.h>
#include <n7OS/sys.h>
#include <n7OS/processus.h>
#include <malloc.h>

extern void handler_IT();
extern void processus1();
extern void idle();

// Handler de l'interruption 50 (pour test)
void handler_en_C() {
    printf("Interruption 50 recu!\n");
}

// Initialisation de l'interruption 50
void init_irq() {
    init_irq_entry(50,(uint32_t)&handler_IT);
}


void kernel_start(void)
{
    // Initialisation des processus
    init();

    // Création des processus
    create_proc("Processus1",&processus1);

    // Initialisation du timer
    init_timer();

    // Initialisation des appels systèmes
    init_syscall();

    // Activation des interruptions
    sti();

    // Ecran d'accueil
    printf("\f");
    printf("Bienvenu sur Minishell!\nVersion O.1\n");
    printf("\n");

    idle();

    while (1) {
        // Affichage du timer
        display_time();    
        

        hlt();
    }
}
