#ifndef TIME_H
#define TIME_H
#include <inttypes.h>

typedef struct time_t {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} time_t;

void init_timer();
void getTimeElapsed(time_t* des);


#endif