#include <stdio.h>
#include <stdbool.h>
#include <n7OS/processus.h>
#include <n7OS/console.h>


// Processus qui gère la mise à jour de l'horloge
void idle(){
    int j=0;
    while(true){
        display_time(); 
        // Petite pause pour le processus 2
        if(j==600000){
             suspendProcess(2);
        }
        // Et on repart
        if(j==900000){
             addProcess(2);
        }
        j++;
    }
}