#ifndef QUEUE_H
#define QUEUE_H

#include "OS.h"

typedef struct Queue {
    TCB* front;
    TCB* rear;
} Queue;

extern Queue priorityQueues[MAX_PRIORITY];

void Init_Queue(Queue* q);
int Is_Queue_Empty(Queue* q);
void Enqueue(Queue* q, TCB* task);
TCB* Dequeue(Queue* q);

#endif // QUEUE_H