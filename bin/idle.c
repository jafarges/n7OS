#include <stdio.h>
#include <stdbool.h>
#include <n7OS/processus.h>

void idle(){
    while(true){
        printf("3*3=9\n");
        scheduler();
    }
}