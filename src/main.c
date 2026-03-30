#include <stdio.h>
#include <unistd.h>
#include "thread.h"
#include "signal_handler.h"

static void thread_work(void) {
    while (1)
        ;
}

int main(void) {
    printf("PID: %d\n", getpid());

    signal_handler_setup();

    for (int i = 0; i < NUM_THREADS; i++)
        thread_create(i, thread_work);

    current_thread = 0;
    threads[0].state = THREAD_RUNNING;

    printf("[Thread 0] executando\n");
    swapcontext(&main_context, &threads[0].context);

    printf("Todas as threads finalizadas.\n");
    threads_cleanup();

    return 0;
}
