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

void Change_Priority(TCB* task, int priority) {
    if (priority < 0 || priority >= MAX_PRIORITY)
        return;
    TCB* current = priorityQueues[task->prio].front;
    TCB* prev = 0;
    while (current != 0) {
        if (current == task) {
            if (prev != 0) {
                prev->next = current->next;
            } 
            else {
                priorityQueues[task->prio].front = current->next;
            }
            if (current->next == 0) {
                priorityQueues[task->prio].rear = prev;
            }

            // 우선순위 변경 후 새로운 큐에 삽입
            current->prio = priority;
            current->next = 0;
            Enqueue(&priorityQueues[priority], current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void Remove_Task_From_Queue(TCB * task)
{
    // 현재 큐에서 작업을 찾고 제거
    TCB* current = priorityQueues[task->prio].front;
    TCB* prev = 0;
    while (current != 0) {
        if (current == task) {
            // 노드 제거
            if (prev != 0) {
                prev->next = current->next;
            } else {
                priorityQueues[task->prio].front = current->next;
            }
            if (current->next == 0) {
                priorityQueues[task->prio].rear = prev;
            }
            return;
        }
        prev = current;
        current = current->next;
    }
}