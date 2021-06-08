#include <stdio.h>
#include <n7OS/processus.h>
#include <n7OS/console.h>
#include <n7OS/time.h>

void processus2() {
  for (int k=0;k<10000;k++){
    lock();
        setcursorpos(5,0);    
        printf("Bonjour du processus 2, k=%d",k);
    unlock();
  }
  exit();
}
