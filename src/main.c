#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include "thread.h"
#include "scheduler.h"
#include "signal_handler.h"
#include "concurrency.h"

#define QUANTUM_US 1000000

static pid_t start_signal_sender(void) {
    pid_t pid = fork();

    if (pid == 0) {
        pid_t parent = getppid();
        while (1) {
            usleep(QUANTUM_US);
            if (kill(parent, SIGUSR1) == -1)
                _exit(0);
        }
    }

    return pid;
}

int main(int argc, char *argv[]) {
    int safe_mode = 0;

    if (argc > 1 && strcmp(argv[1], "--safe") == 0)
        safe_mode = 1;

    void (*work_fn)(void) = safe_mode ? thread_function_safe : thread_function_unsafe;

    printf("Modo: %s\n", safe_mode ? "SAFE" : "UNSAFE");

    signal_handler_setup();
    shared_array_reset();

    for (int i = 0; i < NUM_THREADS; i++)
        thread_create(i, work_fn);

    pid_t sender = start_signal_sender();

    current_thread = 0;
    threads[0].state = THREAD_RUNNING;
    printf("[Thread 0] executando\n");
    swapcontext(&main_context, &threads[0].context);

    while (1) {
        int next = scheduler_next();
        if (next < 0)
            break;
        current_thread = next;
        threads[next].state = THREAD_RUNNING;
        printf("[Thread %d] executando\n", next);
        swapcontext(&main_context, &threads[next].context);
    }

    kill(sender, SIGTERM);
    waitpid(sender, NULL, 0);

    printf("Todas as threads finalizadas.\n");
    shared_array_verify();
    threads_cleanup();

    return 0;
}
