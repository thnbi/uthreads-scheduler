#include <stdio.h>
#include <signal.h>
#include <string.h>
#include "concurrency.h"
#include "thread.h"

static int shared_array[SHARED_ARRAY_SIZE];

void shared_array_reset(void) {
    memset(shared_array, 0, sizeof(shared_array));
}

void thread_function_unsafe(void) {
    for (int iter = 0; iter < ITERATIONS; iter++) {
        for (int i = 0; i < SHARED_ARRAY_SIZE; i++) {
            int temp = shared_array[i];
            temp = temp + 1;
            shared_array[i] = temp;
        }
    }
}

void thread_function_safe(void) {
    sigset_t block, prev;
    sigemptyset(&block);
    sigaddset(&block, SIGUSR1);

    for (int iter = 0; iter < ITERATIONS; iter++) {
        sigprocmask(SIG_BLOCK, &block, &prev);
        for (int i = 0; i < SHARED_ARRAY_SIZE; i++) {
            int temp = shared_array[i];
            temp = temp + 1;
            shared_array[i] = temp;
        }
        sigprocmask(SIG_SETMASK, &prev, NULL);
    }
}

void shared_array_verify(void) {
    int expected = NUM_THREADS * ITERATIONS;
    int errors = 0;

    for (int i = 0; i < SHARED_ARRAY_SIZE; i++) {
        if (shared_array[i] != expected)
            errors++;
    }

    if (errors == 0) {
        printf("Resultado: CORRETO (todos os %d elementos = %d)\n",
               SHARED_ARRAY_SIZE, expected);
    } else {
        printf("Resultado: INCORRETO (%d de %d elementos divergem do esperado %d)\n",
               errors, SHARED_ARRAY_SIZE, expected);
        printf("Exemplos: [0]=%d [1]=%d [2]=%d\n",
               shared_array[0], shared_array[1], shared_array[2]);
    }
}
