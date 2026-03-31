#include <stdio.h>
#include <stdlib.h>
#include "thread.h"
#include "ready_queue.h"

Thread threads[NUM_THREADS];
int current_thread = -1;
ucontext_t main_context;

static void thread_entry(int id) {
    threads[id].work_fn();
    threads[id].state = THREAD_FINISHED;
    printf("[Thread %d] finalizada\n", id);
}

void thread_create(int id, void (*function)(void)) {
    Thread *t = &threads[id];

    t->id = id;
    t->state = THREAD_READY;
    t->work_fn = function;
    t->stack = malloc(STACK_SIZE);

    if (!t->stack) {
        perror("malloc");
        exit(1);
    }

    getcontext(&t->context);
    t->context.uc_stack.ss_sp = t->stack;
    t->context.uc_stack.ss_size = STACK_SIZE;
    t->context.uc_link = &main_context;
    makecontext(&t->context, (void (*)(void))thread_entry, 1, id);

    ready_queue_push(t);
}

void threads_cleanup(void) {
    for (int i = 0; i < NUM_THREADS; i++) {
        free(threads[i].stack);
        threads[i].stack = NULL;
    }
}
