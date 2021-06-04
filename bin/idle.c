#include <stdio.h>
#include <stdbool.h>
#include <n7OS/processus.h>
#include <n7OS/console.h>

void idle(){
    int j=0;
    while(true){
        display_time(); 
        //scheduler(false);
        if(j==300000){
            suspendProcess(1);
        }
        if(j==600000){
            addProcess(1);
        }
        if(j==900000){
            suspendProcess(1);
        }
        j++;
    }
}