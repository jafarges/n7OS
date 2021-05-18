#ifndef PROCESSUS_H
#define PROCESSUS_H

#define MAX_PROCESS 128
#define MAX_NAME_SIZE 64

#include <inttypes.h>

typedef enum {ELU, PRET_ACTIF, PRET_SUSPENDU, BLOQUE_ACTIF, BLOQUE_SUSPENDU} PROCESS_STATE;
typedef uint16_t PROCESS_ID;
typedef uint32_t PROCESS_CTX[5];

typedef struct process_t {
        char prcName[MAX_NAME_SIZE];
        PROCESS_STATE prcState;
        PROCESS_ID prcID;
        PROCESS_CTX prcReg;
} process_t;

typedef process_t process_table[MAX_PROCESS];

typedef void (*myFunction)();

void init();

void addProcess(PROCESS_ID pid);
void removeProcess(PROCESS_ID pid);

void scheduler();
void* stop_process_uc();

PROCESS_ID fork(const char* name, myFunction mfunc);
PROCESS_ID getpid();

#endif