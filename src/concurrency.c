#include <stdio.h>
#include <string.h>
#include "concurrency.h"
#include "semaphore.h"
#include "thread.h"

static int shared_array[SHARED_ARRAY_SIZE];
static Semaphore mutex;

void shared_array_reset(void) {
    memset(shared_array, 0, sizeof(shared_array));
    sem_init_(&mutex, 1);
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
    for (int iter = 0; iter < ITERATIONS; iter++) {
        sem_wait_(&mutex);
        for (int i = 0; i < SHARED_ARRAY_SIZE; i++) {
            int temp = shared_array[i];
            temp = temp + 1;
            shared_array[i] = temp;
        }
        sem_post_(&mutex);
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
