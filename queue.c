#include "queue.h"

Queue priorityQueues[MAX_PRIORITY];

// 큐 초기화
void init_queue(Queue* q) {
    q->front = q->rear = 0;
}

// 스케줄러 초기화
void init_scheduler() {
    int i;
	for (i = 0; i < MAX_PRIORITY; i++) {
        init_queue(&priorityQueues[i]);
    }
}

// 큐가 비어있는지 확인
int is_queue_empty(Queue* q) {
    return q->front == 0;
}

// 큐에 task 추가
void enqueue(Queue* q, TCB* task) {
    if (q->rear == 0) {
        q->front = q->rear = task;
    } else {
        q->rear->next = task;
        q->rear = task;
    }
    task->next = 0;
}

// 큐에서 task 제거
TCB* dequeue(Queue* q) {
    if (is_queue_empty(q)) {
        return 0;
    }
    TCB* task = q->front;
    q->front = q->front->next;
    if (q->front == 0) {
        q->rear = 0;
    }
    return task;
}

// 다음 실행할 task 가져오기
TCB* get_next_task() {
	int i;
    for (i = 0; i < MAX_PRIORITY; i++) {
        if (!is_queue_empty(&priorityQueues[i])) {
			TCB* initTask = dequeue(&priorityQueues[i]);
            TCB* task = initTask;
			do{
                if (task->state == STATE_READY) {
                    enqueue(&priorityQueues[i], task); // 동일한 우선순위의 맨 끝으로 이동
                    return task;
                }
                enqueue(&priorityQueues[i], task); // 다시 큐에 넣어줌
                task = dequeue(&priorityQueues[i]);
            } while(initTask != task);
        }
    }
    return 0; // 실행할 task가 없음
}