#ifndef QUEUE_H
#define QUEUE_H

#include "OS.h"

#define NODE_LIST_SIZE 100

#define SIZEOF_INT 4
#define SIZEOF_CHAR 1
#define SIZEOF_STRING 4
#define SIZEOF_TCB_PTR 4

typedef enum {
    INT,
    CHAR,
    STRING,
    TCB_PTR,
    DEFAULT
} DataType;

typedef struct Node {
    void* data;
    DataType type;
    struct Node* next;
} Node;

typedef struct Queue {
    Node* front;
    Node* rear;
    int max_element;
} Queue;

extern Queue priorityQueues[MAX_PRIORITY];

// Queue* Create_Queue();
void Init_Node_List();
void Init_Queue(Queue* q);
int Is_Queue_Empty(Queue* q);
void Enqueue(Queue* q, void* data, DataType type);
Node* Dequeue(Queue* q);

// void Change_Priority(TCB* task, int priority);
// void Remove_Task_From_Queue(TCB * task);

#endif // QUEUE_H