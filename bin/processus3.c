#include <stdio.h>
#include <n7OS/processus.h>
#include <n7OS/console.h>
#include <unistd.h>
void processus3() {
  int k=0;
  while (k<90000){
    lock();
        setcursorpos(6,0);    
        printf("Bonjour du processus 3, k=%d",k);
        k +=2;
    unlock();
  }
  //shutdown(1);
  exit();
}

