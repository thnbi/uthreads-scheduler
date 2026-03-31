#include <stddef.h>
#include "ready_queue.h"
#include "scheduler.h"

int scheduler_next(void) {
    Thread *t = ready_queue_pop();
    if (!t)
        return -1;
    return t->id;
}
