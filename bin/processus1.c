#include <stdio.h>
#include <n7OS/processus.h>

void processus1() {
  for (;;){
    //cli au début de timer pour les processus
    //sti à la fin avant le ctx switch
    printf("Hello, world from P1\n");
    scheduler();
  }
}
