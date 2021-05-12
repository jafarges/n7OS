#ifndef PROCESSUS_H
#define PROCESSUS_H

#define MAX_PROCESS 128

#include <inttypes.h>

typedef enum {ELU, PRET_ACTIF, PRET_SUSPENDU, BLOQUE_ACTIF, BLOQUE_SUSPENDU} PROCESS_STATE;
typedef uint16_t PROCESS_ID;

typedef struct process_t {
        void* stack;
        PROCESS_STATE prcState;
        PROCESS_ID prcID;
} process_t;

typedef process_t process_table[MAX_PROCESS];

void addProcess(PROCESS_ID pid);
void removeProcess(PROCESS_ID pid);

void scheduler();
void* stop_process_uc();

PROCESS_ID creer();
PROCESS_ID getpid();

#endif