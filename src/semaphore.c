#include <signal.h>
#include <stddef.h>
#include "semaphore.h"
#include "thread.h"

void sem_init_(Semaphore *s, int value) {
    s->value = value;
    s->wait_count = 0;
}

void sem_wait_(Semaphore *s) {
    sigset_t block, prev;
    sigemptyset(&block);
    sigaddset(&block, SIGUSR1);
    sigprocmask(SIG_BLOCK, &block, &prev);

    while (s->value <= 0) {
        s->waiting[s->wait_count++] = current_thread;
        threads[current_thread].state = THREAD_BLOCKED;
        sigprocmask(SIG_SETMASK, &prev, NULL);
        swapcontext(&threads[current_thread].context, &main_context);
        sigprocmask(SIG_BLOCK, &block, &prev);
    }

    s->value--;
    sigprocmask(SIG_SETMASK, &prev, NULL);
}

void sem_post_(Semaphore *s) {
    sigset_t block, prev;
    sigemptyset(&block);
    sigaddset(&block, SIGUSR1);
    sigprocmask(SIG_BLOCK, &block, &prev);

    s->value++;

    if (s->wait_count > 0) {
        int tid = s->waiting[0];
        for (int i = 1; i < s->wait_count; i++)
            s->waiting[i - 1] = s->waiting[i];
        s->wait_count--;
        threads[tid].state = THREAD_READY;
    }

    sigprocmask(SIG_SETMASK, &prev, NULL);
}
