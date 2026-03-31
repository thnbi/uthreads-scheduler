#include <stddef.h>
#include "ready_queue.h"

static Thread *head;
static Thread *tail;

void ready_queue_push(Thread *t) {
    t->next = NULL;
    if (tail)
        tail->next = t;
    else
        head = t;
    tail = t;
}

Thread *ready_queue_pop(void) {
    if (!head)
        return NULL;

    Thread *t = head;
    head = head->next;
    if (!head)
        tail = NULL;
    t->next = NULL;
    return t;
}
