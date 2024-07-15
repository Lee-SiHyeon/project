#ifndef QUEUE_H
#define QUEUE_H

#include "OS.h"

#define MAX_PRIORITY 10
#define MAX_TASKS 100

typedef struct Queue {
    TCB* front;
    TCB* rear;
} Queue;

extern Queue priorityQueues[MAX_PRIORITY];

void init_queue(Queue* q);
void init_scheduler();
int is_queue_empty(Queue* q);
void enqueue(Queue* q, TCB* task);
TCB* dequeue(Queue* q);
TCB* get_next_task();

#endif // QUEUE_H