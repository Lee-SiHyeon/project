#include "queue.h"
#include "device_driver.h"

Node node_list[NODE_LIST_SIZE];

// Queue* Create_Queue()

// 큐 초기화
void Init_Queue(Queue* q) {
    q->front = q->rear = 0;
}

// 큐가 비어있는지 확인
int Is_Queue_Empty(Queue* q) {
    return q->front == 0;
}

void Init_Node_List() {
    int i;
    for(i=0; i<NODE_LIST_SIZE; i++)
	{
		node_list[i].next = 0;
	}
}

// 큐에 task 추가
void Enqueue(Queue* q, void* data, DataType type){
    int i;
    for(i = 0; i < NODE_LIST_SIZE; i++){
		if(node_list[i].next == 0){
			node_list[i].data = data;
            node_list[i].type = type;
            node_list[i].next = &node_list[i];
            break;
		}
	}
    if (q->rear == 0) {
        q->front = q->rear = &node_list[i];
    } else {
         
        q->rear->next = &node_list[i];
        q->rear = &node_list[i];
    }
}

// 큐에서 task 제거
Node* Dequeue(Queue* q) {
    if (Is_Queue_Empty(q)) {
        return 0;
    }

    Node* node = q->front;
    q->front = q->front->next;
    node->next = 0;
    
    if (q->front == 0) {
        q->rear = 0; // If queue becomes empty, rear should be NULL
    }
    return node;
}

// void Change_Priority(TCB* task, int priority) {
//     if (priority < 0 || priority >= MAX_PRIORITY)
//         return;
//     TCB* current = priorityQueues[task->prio].front;
//     TCB* prev = 0;
//     while (current != 0) {
//         if (current == task) {
//             if (prev != 0) {
//                 prev->next = current->next;
//             } 
//             else {
//                 priorityQueues[task->prio].front = current->next;
//             }
//             if (current->next == 0) {
//                 priorityQueues[task->prio].rear = prev;
//             }

//             // 우선순위 변경 후 새로운 큐에 삽입
//             current->prio = priority;
//             current->next = 0;
//             Enqueue(&priorityQueues[priority], current);
//             return;
//         }
//         prev = current;
//         current = current->next;
//     }
// }

// void Remove_Task_From_Queue(TCB * task)
// {
//     // 현재 큐에서 작업을 찾고 제거
//     TCB* current = priorityQueues[task->prio].front;
//     TCB* prev = 0;
//     while (current != 0) {
//         if (current == task) {
//             // 노드 제거
//             if (prev != 0) {
//                 prev->next = current->next;
//             } else {
//                 priorityQueues[task->prio].front = current->next;
//             }
//             if (current->next == 0) {
//                 priorityQueues[task->prio].rear = prev;
//             }
//             return;
//         }
//         prev = current;
//         current = current->next;
//     }
// }