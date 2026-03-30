#include "thread.h"
#include "scheduler.h"

int scheduler_next(void) {
    for (int i = 1; i <= NUM_THREADS; i++) {
        int idx = (current_thread + i) % NUM_THREADS;
        if (threads[idx].state == THREAD_READY || threads[idx].state == THREAD_RUNNING)
            return idx;
    }
    return -1;
}
