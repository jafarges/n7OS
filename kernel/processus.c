#include <n7OS/processus.h>

process_table prc_table;
PROCESS_ID currentPID;

PROCESS_ID allouer_pid(){
    static PROCESS_ID crPid=0;
    crPid++;
    if(!(crPid<MAX_PROCESS))
        return -1;
    else
        return crPid;
}

void* init_stack(){
    return 0x0;
}

PROCESS_ID creer()
{
    PROCESS_ID pid;
    if((pid=allouer_pid())==-1){
        printf("PID MAX REACHED\n");
        return -1;
    }

    prc_table[pid].prcID = pid;
    prc_table[pid].stack = init_stack();
    prc_table[pid].prcState = PRET_SUSPENDU;

    
}

void addProcess(PROCESS_ID pid){

}

void removeProcess(PROCESS_ID pid){

}

void scheduler(){

}

void* stop_process_uc(){

}

PROCESS_ID getpid(){
    return currentPID;
}

