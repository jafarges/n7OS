#ifndef TIME_H
#define TIME_H
#include <inttypes.h>

// Structure d'un timer :
// => Heures
// => Minutes
// => Secondes
typedef struct time_t {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} time_t;

// Initialisation du timer (et de son interruption associée)
void init_timer();

// Récupération du temps écoulé depuis le démarrage du noyau (dans des)
void getTimeElapsed(time_t* des);

// Retourne le nombre de millisecondes écoulées depuis le démarrage du système
int getMillis();

#endif