/*
 * game_object_queue.h
 *
 *  Created on: 2024. 7. 18.
 *      Author: yeeun99.kim
 */

#ifndef GAME_OBJECT_QUEUE_H
#define GAME_OBJECT_QUEUE_H

#include <stdbool.h>
#include "lcd_test.h"

#define QUEUE_MAX_SIZE 100

typedef struct {
    GameObject queue[QUEUE_MAX_SIZE];
    int front;
    int rear;
    int size;
} GameObjectQueue;

// Function declarations
extern void initQueue(GameObjectQueue *q);
extern bool isQueueEmpty(GameObjectQueue *q);
extern bool isQueueFull(GameObjectQueue *q);
extern bool enqueue(GameObjectQueue *q, GameObject item);
extern bool dequeue(GameObjectQueue *q, GameObject *item);
extern int queueSize(GameObjectQueue *q);


#endif // GAME_OBJECT_QUEUE_H


