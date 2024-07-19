#include "queue.h"
#include "device_driver.h"

extern Queue ready_Queues[10];
Node node_list[NODE_LIST_SIZE];
int node_cnt =NODE_LIST_SIZE;
Queue queue_list[20];
extern volatile unsigned int sys_cnt;

// 큐 초기화
void Init_Queue(Queue* q) {
    q->front = q->rear = 0;
    q->element_cnt = 0;
}

// 큐가 비어있는지 확인
int Is_Queue_Empty(Queue* q) {
    if(q->element_cnt==0) return 1;
    return 0;
}

int Is_Queue_Using(Queue* q){
    if(q->is_using ==1) return 1;
    return 0;
}

void Init_Node_List() {
    int i;
    for(i=0; i<NODE_LIST_SIZE; i++)
	{
		node_list[i].next = 0;
	}
}

// 큐에 task 추가
int Enqueue(Queue* q, void* data, DataType type){
    int i;
    if( q->element_cnt >= q->element_max ) return 0;
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
    q->element_cnt++;
    return 1;
}

// 큐에서 task 제거
Node* Dequeue(Queue* q) {
    unsigned int start_tick = sys_cnt;
    int data_ready = !Is_Queue_Empty(q);
    if (data_ready == 0) {
        while((sys_cnt - start_tick) < DEQUEUE_TIMEOUT)
        {
            // Uart_Printf("Dequeue timeout = %d\n", sys_cnt);
            if (!Is_Queue_Empty(q))
            {
                data_ready = 1;
                break;
            }
        }
    }
    if (data_ready == 0)
        return 0;

    Node* node = q->front;
    if(q->front == q->front->next){
        // this queue will be empty after executing this dequeue.
        q->front = 0; 
    }else{
        q->front = q->front->next;
    }
    
    node->next = 0;
    
    if (q->front == 0) {
        q->rear = 0; // If queue becomes empty, rear should be NULL
    }

    q->element_cnt--;
    return node;
}

Queue* Create_Queue(int element_max)
{
    int i;
    if( element_max ==0){
        Uart_Printf("[Create Queue] invalid param, element_max ==0\n");
        return 0;
    }
    if( element_max > node_cnt ){
        // can not allocate enough nodes.
        Uart_Printf("[Create_Queue] doesn't have enough nodes, \
        %d > %d\n", element_max, node_cnt);
        return 0;
    }
    Uart_Printf("element_max = %d, node_cnt = %d\n", element_max, node_cnt);
    for(i=0; i<sizeof(queue_list)/sizeof(queue_list[0]); i++){
        //find empty queue
        if(!Is_Queue_Using(&queue_list[i])){
            node_cnt -= element_max;
            queue_list[i].front =0;
            queue_list[i].rear =0;
            queue_list[i].element_cnt =0;
            queue_list[i].is_using = 1;
            queue_list[i].element_max =element_max;
            return &queue_list[i];
        } 
    }
    Uart_Printf("[Create_Queue] doesn't have enough queue\n");
    return (Queue*) 0;
}

// void Change_Priority(TCB* task, int priority) {
//     if (priority < 0 || priority >= MAX_PRIORITY)
//         return;
//     TCB* current = ready_Queues[task->prio].front;
//     TCB* prev = 0;
//     while (current != 0) {
//         if (current == task) {
//             if (prev != 0) {
//                 prev->next = current->next;
//             } 
//             else {
//                 ready_Queues[task->prio].front = current->next;
//             }
//             if (current->next == 0) {
//                 ready_Queues[task->prio].rear = prev;
//             }

//             // 우선순위 변경 후 새로운 큐에 삽입
//             current->prio = priority;
//             current->next = 0;
//             Enqueue(&ready_Queues[priority], current);
//             return;
//         }
//         prev = current;
//         current = current->next;
//     }
// }

// void Remove_Task_From_Queue(TCB * task)
// {
//     // 현재 큐에서 작업을 찾고 제거
//     TCB* current = ready_Queues[task->prio].front;
//     TCB* prev = 0;
//     while (current != 0) {
//         if (current == task) {
//             // 노드 제거
//             if (prev != 0) {
//                 prev->next = current->next;
//             } else {
//                 ready_Queues[task->prio].front = current->next;
//             }
//             if (current->next == 0) {
//                 ready_Queues[task->prio].rear = prev;
//             }
//             return;
//         }
//         prev = current;
//         current = current->next;
//     }
// }