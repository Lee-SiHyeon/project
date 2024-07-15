#include "queue.h"

// 큐 초기화
void Init_Queue(Queue* q) {
    q->front = q->rear = 0;
}

// 큐가 비어있는지 확인
int Is_Queue_Empty(Queue* q) {
    return q->front == 0;
}

// 큐에 task 추가
void Enqueue(Queue* q, TCB* task) {
    if (q->rear == 0) {
        q->front = q->rear = task;
    } else {
        q->rear->next = task;
        q->rear = task;
    }
    task->next = 0;
}

// 큐에서 task 제거
TCB* Dequeue(Queue* q) {
    if (Is_Queue_Empty(q)) {
        return 0;
    }
    TCB* task = q->front;
    q->front = q->front->next;
    if (q->front == 0) {
        q->rear = 0;
    }
    return task;
}