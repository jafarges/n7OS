#include <stdio.h>
#include <n7OS/processus.h>

void processus1() {
  int i=0;
  for (;;){
    //cli au début de timer pour les processus
    //sti à la fin avant le ctx switch
    printf("[%d]\r",i++);
    //scheduler(false);
  }
}
