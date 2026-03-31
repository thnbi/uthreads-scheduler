#ifndef CONCURRENCY_H
#define CONCURRENCY_H

#define SHARED_ARRAY_SIZE 1000
#define ITERATIONS 1000

void thread_function_unsafe(void);
void thread_function_safe(void);
void shared_array_reset(void);
void shared_array_verify(void);

#endif
