#include <n7OS/processus.h>
#include <malloc.h>
#include <inttypes.h>
#include <file.h> 
#include <string.h>
#include <n7OS/cpu.h>
#include <n7OS/processor_structs.h>

extern void ctx_sw(void* ctx_old,void* ctx_new);

// Table des processus prêts
process_table prc_table;

// File des processus prêts
process_ready prc_ready;

// Nombre de processus prêts
int nb_ready_proc=0;

// ID du processus courant
PROCESS_ID currentPID;

// Selecteur de processus (dans la file)
int proc_ready_sel=0;

void init(){
    // Initialement, processus IDLE en cours d'exécution
    currentPID = 0; 
    memset(prc_table,0,sizeof(process_t)*MAX_PROCESS);

    // Nom du processus
    strcpy(prc_table[0].prcName,"IDLE");
   
    // PID
    prc_table[0].prcID = 0;

    // Processus initalement ACTIF
    prc_table[0].prcState = ELU;

    // Ajout dans la file des processus prêt
    addProcess(0);
}

// Retourne le prochain PID disponible
// ou -1 si l'on ne peut plus crée de processus
PROCESS_ID allouer_pid(){
    static PROCESS_ID crPid=0;
    crPid++; // On prend le prochain PID disponible
    if(!(crPid<MAX_PROCESS))
        return -1;
    else
        return crPid;
}

// Créer le processus de nom name, et de fonction mfunc
PROCESS_ID create_proc(const char* name, myFunction mfunc)
{
    PROCESS_ID pid;
    if((pid=allouer_pid())==-1){
        printf("PID MAX REACHED\n");
        return -1;
    }

    // Nom du processus
    strncpy(prc_table[pid].prcName,name,MAX_NAME_SIZE);
    
    // On insère NULL à la fin de la chaîne de caractères pour
    // se protéger d'éventuels attaques (bien que la chaîne name
    // ne provienne pas d'une entrée au clavier)
    *(prc_table[pid].prcName+MAX_NAME_SIZE-1) = 0;

    // PID
    prc_table[pid].prcID = pid;

    // Processus initalement prêt et actif
    prc_table[pid].prcState = PRET_ACTIF;

    // On initialise le contexte
    memset(prc_table[pid].prcReg,0,sizeof(uint32_t)*5);

    // On définit la fonction à appeler
    prc_table[pid].prcStack[STACK_SIZE-1] = (uint32_t)mfunc;
    
    // Et le pointeur sur la pile du processus
    prc_table[pid].prcReg[1]=(uint32_t)&prc_table[pid].prcStack[STACK_SIZE-1];

    // Enfin, on ajoute le processus à la file des processus prêt
    addProcess(pid);

    return -1;
}


// Ajoute un processus à la file des processus prêts
void addProcess(PROCESS_ID pid){
    if(nb_ready_proc<MAX_PROCESS){
        // On sauvegarde dans la table des processus la 
        // position du processus à ajouter, dans la queue
        prc_table[pid].prcQueueIdx=nb_ready_proc;

        // Puis on le range dans la queue, à cette position
        prc_ready[nb_ready_proc++]=&prc_table[pid];
    }
}

void suspendProcess(PROCESS_ID pid){
    prc_table[pid].prcState=PRET_SUSPENDU;
    int qLoc = prc_table[pid].prcQueueIdx;

    for(int k=(++qLoc);k<nb_ready_proc;k++){
        prc_ready[k-1]=prc_ready[k];
    }
    nb_ready_proc--;
    prc_ready[nb_ready_proc] = NULL;

    // Attention au cas dans lequel le processus suspendu
    // est celui en cours d'exécution
    if(pid==currentPID){
        // On décale le sélecteur de processus 
        // de un rang afin de ne pas "skip" le processus
        // qui suit
        if(proc_ready_sel==0){
            proc_ready_sel=nb_ready_proc-1;
        }else{
            proc_ready_sel--;
        }
        scheduler(true);
    }
}


// On récupère le prochain processus de la file à exécuter
// (puis on déplace le sélecteur de processus)
PROCESS_ID nextProcess(){
    proc_ready_sel = (proc_ready_sel + 1)%nb_ready_proc;
    PROCESS_ID retID = prc_ready[proc_ready_sel]->prcID;
    return retID;
}

void scheduler(bool onSuspend){
    if(nb_ready_proc>0){
        PROCESS_ID oldPID = currentPID;

        currentPID = nextProcess();                   

        prc_table[oldPID].prcState = onSuspend ? PRET_SUSPENDU : PRET_ACTIF ;
        prc_table[currentPID].prcState = ELU;

        sti();
        ctx_sw(prc_table[oldPID].prcReg,prc_table[currentPID].prcReg);
    }
}

PROCESS_ID getpid(){
    return currentPID;
}
