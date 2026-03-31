#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "thread.h"

typedef struct {
    int value;
    int waiting[NUM_THREADS];
    int wait_count;
} Semaphore;

void sem_init_(Semaphore *s, int value);
void sem_wait_(Semaphore *s);
void sem_post_(Semaphore *s);

#endif
