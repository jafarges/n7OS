#include <n7OS/processus.h>
#include <malloc.h>
#include <inttypes.h>
#include <file.h> 
#include <string.h>
#include <n7OS/cpu.h>
#include <n7OS/processor_structs.h>
#include <n7OS/time.h>
#include <n7OS/console.h>

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

// Verrou de la sémaphore
bool sync_lock=false;

// Demande d'ordonnancement reçue lors d'un verrou (sémaphore)
bool schedule_asked=false;

// Temps système (en ms) à partir duquel l'ordonnanceur effectuera 
// le changement de processus (par rapport au nombre de ms écoulées depuis le démarrage)
int next_scheduling=10;

// Nombre de processus crés
int nb_proc_created=0;

// Vérouillage de la sémaphore
void lock(){
    sync_lock=true;
}

// Dévérouillage de la sémaphore
void unlock(){
    sync_lock=false;
    if(schedule_asked){
        schedule_asked=false;
        next_scheduling = getMillis() + 10;
        sw_proc(true);
    }
}


char* getProcStateStr(PROCESS_STATE pState){
    switch(pState){
        case ELU:
            return "ELU";
        case PRET_ACTIF:
            return "ACTIF";
        case PRET_SUSPENDU:
            return "SUSPENDU";
        case EXITED:
            return "TERMINE";
        default:
            return "INCONNU";
    }
}


void process_manager(){
    // Affiche les informations des processus crés dans un tableau
    // (10 au maximum)
    int proc2show;

    while(true){
        proc2show =nb_proc_created;
        if(proc2show>10){
            proc2show = 10;
        }
        lock();
            setcursorpos(7,0);
            printf("| ID |    NOM     |    ETAT    |\n");
            for(int lF = 0;lF<proc2show;lF++){
                printf("| %d  | %10.10s | %10.10s |\n",prc_table[lF].prcID,prc_table[lF].prcName,getProcStateStr(prc_table[lF].prcState));
            }
        unlock();  
    }
}

void init(){
    nb_proc_created=1;

    // Initialement, processus IDLE en cours d'exécution
    // (le processus qui met à jour l'horloge)
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

    // Ajout du processus permettant de visualiser les processus
    // crés
    create_proc("PROC_MGR",&process_manager);
}

// Retourne le prochain PID disponible
// ou -1 si l'on ne peut plus crée de processus
PROCESS_ID allouer_pid(){
    if(!(nb_proc_created<MAX_PROCESS))
        return -1;
    else
        return nb_proc_created++;
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


// Supprime le processus de PID donné de la liste des processus prêt
void removeProcess(int pid){
    int qLoc = prc_table[pid].prcQueueIdx;

    for(int k=(++qLoc);k<nb_ready_proc;k++){
        // On déplace le processus dans la file des processus prêt
        prc_ready[k]->prcQueueIdx = k-1;

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

        sw_proc(false);
    }
}

void scheduler(int ms_from_st){
    // Ordonnancement par quantum de temps, toutes les 10ms
    if(ms_from_st == next_scheduling){
        if(sync_lock){
            schedule_asked=true;
            // Pas de prochain ordonancement prévu, c'est lors du
            // déverrouillage de la sémaphore que l'on 
            // effectuera le changement de processus élu
        }else{
            next_scheduling+=10; // Prochain ordonancement dans 10ms;
            sw_proc(true);
        }
    }
}



void suspendProcess(PROCESS_ID pid){
    prc_table[pid].prcState=PRET_SUSPENDU;
    removeProcess(pid);
}



// On récupère le prochain processus de la file à exécuter
// (puis on déplace le sélecteur de processus)
PROCESS_ID nextProcess(){
    proc_ready_sel = (proc_ready_sel + 1)%nb_ready_proc;
    PROCESS_ID retID = prc_ready[proc_ready_sel]->prcID;
    return retID;
}

void sw_proc(bool stillActive){
    if(nb_ready_proc>0){
        PROCESS_ID oldPID = currentPID;

        currentPID = nextProcess();                   

        // On vérifie si le processus anciennement élu reste actif
        if(stillActive){
            prc_table[oldPID].prcState = PRET_ACTIF ;
        }

        prc_table[currentPID].prcState = ELU;

        sti();
        ctx_sw(prc_table[oldPID].prcReg,prc_table[currentPID].prcReg);
    }
}

PROCESS_ID getpid(){
    return currentPID;
}

void exit(){
    // On controle que l'on ne cherche pas à quitter le processus d'horloge 
    if(currentPID != 0){
        prc_table[currentPID].prcState = EXITED;
        removeProcess(currentPID);
    }    
}