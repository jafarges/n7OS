#include <stdio.h>
#include <n7OS/processus.h>
#include <n7OS/console.h>
#include <n7OS/time.h>

void processus1() {
  int i=0;
  for (;;){
    lock();
        setcursorpos(3,0);    
        printf("Processus 1 : compteur i=%d\r",i++);
    unlock();
  }
  exit();
}
