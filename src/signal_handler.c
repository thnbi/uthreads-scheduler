#include <signal.h>
#include <stdlib.h>
#include "thread.h"
#include "scheduler.h"
#include "dispatcher.h"
#include "signal_handler.h"

static void handle_sigusr1(int sig) {
    (void)sig;

    int next = scheduler_next();

    if (next < 0) {
        threads[current_thread].state = THREAD_FINISHED;
        setcontext(&main_context);
    }

    if (next != current_thread)
        dispatcher_switch(next);
}

void signal_handler_setup(void) {
    struct sigaction sa;
    sa.sa_handler = handle_sigusr1;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);
}
