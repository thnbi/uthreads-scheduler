#ifndef THREAD_H
#define THREAD_H

#include <ucontext.h>

#define NUM_THREADS 5
#define STACK_SIZE 16384

typedef enum {
    THREAD_READY,
    THREAD_RUNNING,
    THREAD_FINISHED
} ThreadState;

typedef struct {
    ucontext_t context;
    char *stack;
    int id;
    ThreadState state;
    void (*work_fn)(void);
} Thread;

extern Thread threads[NUM_THREADS];
extern int current_thread;
extern ucontext_t main_context;

void thread_create(int id, void (*function)(void));
void threads_cleanup(void);

#endif
