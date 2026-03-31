#ifndef READY_QUEUE_H
#define READY_QUEUE_H

#include "thread.h"

void ready_queue_push(Thread *t);
Thread *ready_queue_pop(void);

#endif
