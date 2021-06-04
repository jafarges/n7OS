#include <n7OS/processus.h>
#include <malloc.h>
#include <inttypes.h>
#include <file.h>
#include <string.h>

#define STACK_SIZE 1024

extern void ctx_sw(void* ctx_old,void* ctx_new);

process_table prc_table;
File prc_ready;
PROCESS_ID currentPID;


void init(){
    // No process currently
    currentPID = -1; 
    create(&prc_ready);
}

PROCESS_ID allouer_pid(){
    static PROCESS_ID crPid=0;
    crPid++;
    if(!(crPid<MAX_PROCESS))
        return -1;
    else
        return crPid;
}

uint32_t init_stack(){
    // Do not forget to offset the stack of its total size (for descending stack)
    return (uint32_t)malloc(sizeof(uint32_t)*STACK_SIZE)+STACK_SIZE-1;
}

PROCESS_ID fork(const char* name, myFunction mfunc)
{

    /*PROCESS_ID pid;
    if((pid=allouer_pid())==-1){
        printf("PID MAX REACHED\n");
        return -1;
    }

    strncpy(prc_table[pid].prcName,name,MAX_NAME_SIZE);
    // Insert null at the end of the string 
    *(prc_table[pid].prcName+MAX_NAME_SIZE-1) = 0;

    prc_table[pid].prcID = pid;
    prc_table[pid].prcState = PRET_SUSPENDU;
*/
    // Initialize the context
    //memset(prc_table[pid].prcReg,0,sizeof(uint32_t)*5);

    // Set the stack pointer
   // prc_table[pid].prcReg[1]=init_stack();

  //  uint32_t* stackPointer = (uint32_t*)prc_table[pid].prcReg[1];
    // Define the function to call
    //stackPointer[STACK_SIZE-1] = (uint32_t)mfunc;

    // Add the process in the queue
  //  addProcess(1);

    return -1;
}

void addProcess(PROCESS_ID pid){
    enqueue(&prc_ready,pid);
}

void removeProcess(PROCESS_ID pid){
    del(&prc_ready,pid);
}

void scheduler(){
    if(length(prc_ready)>0){
        PROCESS_CTX old; 
        PROCESS_ID nextPID = dequeue(&prc_ready);
        PROCESS_ID oldPID = currentPID;
                    
        currentPID = nextPID; 

        if(oldPID != -1){
            
            prc_table[oldPID].prcState = PRET_SUSPENDU;
            prc_table[nextPID].prcState = PRET_ACTIF;

            ctx_sw(prc_table[oldPID].prcReg,prc_table[nextPID].prcReg);
        }else{
            ctx_sw(old,prc_table[nextPID].prcReg);
        }
        
    }
}
