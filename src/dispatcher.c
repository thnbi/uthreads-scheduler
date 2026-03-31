#include <stdio.h>
#include "thread.h"
#include "ready_queue.h"
#include "dispatcher.h"

void dispatcher_switch(int next) {
    int prev = current_thread;

    if (threads[prev].state == THREAD_RUNNING) {
        threads[prev].state = THREAD_READY;
        ready_queue_push(&threads[prev]);
    }

    threads[next].state = THREAD_RUNNING;
    current_thread = next;

    printf("--- troca ---\n");
    printf("[Thread %d] executando\n", threads[next].id);

    swapcontext(&threads[prev].context, &threads[next].context);
}
