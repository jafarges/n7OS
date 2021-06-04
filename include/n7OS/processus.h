#ifndef PROCESSUS_H
#define PROCESSUS_H
#include <inttypes.h>

// Nombre de process max
#define MAX_PROCESS 128

// Taille maximale d'un nom de processus
#define MAX_NAME_SIZE 64

// Taille de la pile des processus
#define STACK_SIZE 1024

// Etat possibles pour un processus
typedef enum {ELU, PRET_ACTIF, PRET_SUSPENDU, BLOQUE_ACTIF, BLOQUE_SUSPENDU} PROCESS_STATE;

// Identifiant d'un processus
typedef uint16_t PROCESS_ID;

// Contexte d'un processus (5 registres)
typedef uint32_t PROCESS_CTX[5];

// Type d'une processus, stocké dans sa table
// -> Un nom
// -> Un état
// -> un ID
// -> Un tableau de registres (le contexte du processus)
// -> Un pointeur sur la pile du processus
typedef struct process_t {
        char prcName[MAX_NAME_SIZE]; 
        PROCESS_STATE prcState;
        PROCESS_ID prcID;
        PROCESS_CTX prcReg;
        uint32_t prcStack[STACK_SIZE];
        
} process_t;

// Table des processus
typedef process_t process_table[MAX_PROCESS];

// Liste des processus prêt
typedef process_t* process_ready[MAX_PROCESS];


// Un pointeur sur une fonction (à exécuter par un processus)
typedef void (*myFunction)();

// Initialisation des processus
void init();

// Ajout d'un processus dans la file des processus prêt
void addProcess(PROCESS_ID pid);

// Suppression d'un processus de la file des processus prêt
void removeProcess(PROCESS_ID pid);

// Switching de processus
void scheduler();

// Création d'un processus
PROCESS_ID create_proc(const char* name, myFunction mfunc);

// Récupération de l'ID du processus courant
PROCESS_ID getpid();

#endif