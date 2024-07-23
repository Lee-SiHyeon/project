#ifndef QUEUE_H
#define QUEUE_H

#define NODE_LIST_SIZE 200
#define QUEUE_LIST_SIZE 15
#define DEQUEUE_TIMEOUT 500
#define SIZEOF_INT 4
#define SIZEOF_CHAR 1
#define SIZEOF_STRING 4
#define SIZEOF_TCB_PTR 4

typedef enum {
    DATATYPE_UNUSED =-1,
    INT,
    CHAR,
    STRING,
    TCB_PTR,
    STRUCT_SIGNAL,
    DEFAULT,
    DATATYPE_MAX
} DataType;

typedef enum {
    FALSE = 0,
    TRUE = 1
} bool;

typedef struct Node {
    void* data;
    DataType type;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct Queue {
    Node* front;
    Node* rear;
    char node_start;
    int node_cnt;
    int node_max;
    char is_using;
} Queue;

void Init_Node_List();
void Init_Queue(Queue* q);
int Is_Queue_Empty(Queue* q);
int Enqueue(Queue* q, void* data, DataType type);
Node* Dequeue(Queue* q);

// void Change_Priority(TCB* task, int priority);
// void Remove_Task_From_Queue(TCB * task);

#endif // QUEUE_H
