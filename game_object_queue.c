/*
 * game_object_queue.c
 *
 *  Created on: 2024. 7. 18.
 *      Author: yeeun99.kim
 */


#include "game_object_queue.h"
#include <stdio.h>

void initQueue(GameObjectQueue *q) {
    q->front = 0;
    q->rear = -1;
    q->size = 0;
}

bool isQueueEmpty(GameObjectQueue *q) {
    return q->size == 0;
}

bool isQueueFull(GameObjectQueue *q) {
    return q->size == QUEUE_MAX_SIZE;
}

bool enqueue(GameObjectQueue *q, GameObject item) {
    if (isQueueFull(q)) {
        return false;
    }
    q->rear = (q->rear + 1) % QUEUE_MAX_SIZE;
    q->queue[q->rear] = item;
    q->size++;
    return true;
}

bool dequeue(GameObjectQueue *q, GameObject *item) {
    if (isQueueEmpty(q)) {
        return false;
    }
    *item = q->queue[q->front];
    q->front = (q->front + 1) % QUEUE_MAX_SIZE;
    q->size--;
    return true;
}

int queueSize(GameObjectQueue *q) {
    return q->size;
}




