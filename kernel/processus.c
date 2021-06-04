#include <n7OS/processus.h>
#include <malloc.h>
#include <inttypes.h>
#include <file.h>
#include <string.h>

extern void ctx_sw(void* ctx_old,void* ctx_new);

// Table des processus prêt
process_table prc_table;

// Processus élu
process_ready prc_elected;

File prc_ready;
PROCESS_ID currentPID;
int nb_proc_elected=0;
int proc_elected_sel=0;

void init(){
    // Initialiement, processus IDLE en cours d'exécution
    currentPID = 0; 
    memset(prc_table+1,0,sizeof(process_t)*(MAX_PROCESS-1));

    // Nom du processus
    strcpy(prc_table[0].prcName,"IDLE");
   
    // PID
    prc_table[0].prcID = 0;

    // Processus initalement ACTIF
    prc_table[0].prcState = PRET_ACTIF;

    // Ajout dans la pile des processus prêt
    prc_elected[0] = 0;
    nb_proc_elected++;
}

PROCESS_ID allouer_pid(){
    static PROCESS_ID crPid=0;
    crPid++; // On prend le prochain PID disponible
    if(!(crPid<MAX_PROCESS))
        return -1;
    else
        return crPid;
}

PROCESS_ID create(const char* name, myFunction mfunc)
{
    PROCESS_ID pid;
    if((pid=allouer_pid())==-1){
        printf("PID MAX REACHED\n");
        return -1;
    }

    // Nom du processus
    strncpy(prc_table[pid].prcName,name,MAX_NAME_SIZE);
    
    // On insère NULL à la fin de la chaîne de caractères
    *(prc_table[pid].prcName+MAX_NAME_SIZE-1) = 0;

    // PID
    prc_table[pid].prcID = pid;

    // Processus initalement prêt et suspendu
    prc_table[pid].prcState = PRET_SUSPENDU;

    // On initialise le contexte
    memset(prc_table[pid].prcReg,0,sizeof(uint32_t)*5);

  

  //  uint32_t* stackPointer = (uint32_t*)prc_table[pid].prcReg[1];
    // On définit la fonction à appeler
    prc_table[pid].prcStack[STACK_SIZE-1] = (uint32_t)mfunc;


    prc_elected[nb_proc_elected++]=pid;


    //PROCESS_CTX oldCtx;

    //ctx_sw(oldCtx,prc_table[pid].prcReg);
        
    // Add the process in the queue
    //addProcess(1);

    return -1;
}

void addProcess(PROCESS_ID pid){
    enqueue(&prc_ready,pid);
}

void removeProcess(PROCESS_ID pid){
    del(&prc_ready,pid);
}

/*void scheduler(){
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
}*/

void scheduler(){
    if(nb_proc_elected>0){
        PROCESS_ID nextPID = prc_elected[(++proc_elected_sel) % nb_proc_elected];
        PROCESS_ID oldPID = currentPID;
                    
        currentPID = nextPID; 
      
        prc_table[oldPID].prcState = PRET_SUSPENDU;
        prc_table[nextPID].prcState = PRET_ACTIF;

        ctx_sw(prc_table[oldPID].prcReg,prc_table[nextPID].prcReg);
    }
}
