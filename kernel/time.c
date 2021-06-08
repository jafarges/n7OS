#include <n7OS/time.h>
#include <inttypes.h>
#include <n7OS/cpu.h>
#include <n7OS/irq.h>
#include <stdio.h>
#include <n7OS/processus.h>

#define F_OSC 0x1234BD

int ms_from_start;

extern void handler_TIMER_IT();

void init_timer(){
    ms_from_start=0;

    // Set command : Impulsion generator / Binary encoded frequency
    outb(0x34,0x43);

    // Set frequency for channel 0
    uint16_t target_clk = 1000;
    uint16_t f_ch0 = F_OSC/target_clk;
    outb(f_ch0 & 0xFF,0x40);
    outb(f_ch0>>8,0x40);
  
    // Activating the timer interruption
    outb(inb(0x21)&0xFE,0x21);

    // Binding timer interruption to its handler
    init_irq_entry(0x20,(uint32_t)&handler_TIMER_IT);
}

void getTimeElapsed(time_t* des){
    int currTime = ms_from_start;
    des->hours = ms_from_start / 3600000;
    
    currTime -= des->hours * 3600000;
    des-> minutes = currTime/60000;
    
    currTime -= des->minutes * 60000;

    des->seconds = currTime / 1000;
}

int getMillis(){
    return ms_from_start;
}

// Traitant de l'interruption du timer
void handler_Timer_C(){
    outb(0x20,0x20);
    cli();
    
    ms_from_start++;

    // Appel à l'ordonnanceur
    scheduler(ms_from_start);
}