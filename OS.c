#include "device_driver.h"
#include "OS.h"
#include "queue.h"

// 참고 : 최초 제공된 코드는 완전한 코드가 아님
//      그러므로 추후 RTOS 설계에 따라 보완이 필요함


/* Global Variable */
TCB tcb[MAX_TCB];
char stack[STACK_SIZE] __attribute__((__aligned__(8)));
Queue queue_list[QUEUE_LIST_SIZE];
extern Node node_list[NODE_LIST_SIZE];
volatile TCB* current_tcb;
volatile TCB* next_tcb;
volatile TCB** p_current_tcb;

Queue* ready_Queues[MAX_PRIORITY];
Queue* signaling_Queue;
extern volatile unsigned int sys_cnt;

int mutex_is_init=0;
Mutex mutexs[MAX_MUTEX];
/* Function Prototype */

void OS_Init(void)
{
	_OS_Init_Scheduler();
	int i;

	for(i=0; i<MAX_TCB; i++)
	{
		tcb[i].no_task = i;
	}
	signaling_Queue = _OS_Get_Queue(sizeof(Node)+8, 20);
	Uart_Printf("signaling_Queue = %x\n", signaling_Queue);
	// Mutex init
	for (i = 0; i < MAX_MUTEX; i++) {
        mutexs[i].owner = -1;
		mutexs[i].used = 0;
    }
}

// 스케줄러 초기화
void _OS_Init_Scheduler() {
    int i;
	for(i = 0; i< QUEUE_LIST_SIZE; i++)
	{
		Init_Queue(&queue_list[i]);
	}
	Init_Node_List();
	for (i = 0; i < MAX_PRIORITY; i++) {
        ready_Queues[i] = _OS_Get_Queue(sizeof(Node), 5);
		Uart_Printf("ready_Queue[%d] = %x\n",i, ready_Queues[i]);
    }
}

char* _OS_Get_Stack(int size){
	static char* stack_limit = stack;
	static char* pstack = stack + STACK_SIZE;

	char* ret = pstack;

	size = (size + 7) & ~0x7;

	if(ret - size < stack_limit) return (char*)0;
	pstack -= size;

	return ret;
}

Queue* _OS_Get_Queue(int element_size, int element_max){
	static char* queue_limit = node_list + sizeof(node_list);
	static char node_idx = 0;

	Queue* ret;
	int node_start;
	int node_end;
	int i;
	for(i=0; i<QUEUE_LIST_SIZE; i++){
		if(queue_list[i].is_using ==0){
			
			ret = &queue_list[i];
			ret->front =0;
			ret->rear =0;
			ret->node_cnt=0;
			ret->node_max=0;
			ret->node_start=0;
			ret->is_using=1;
			Uart_Printf("queue_list[%d] is selected %d, %d \n",i, queue_list[i].is_using, ret->is_using);
			break;
		}
	}
	if(node_list[node_idx].next == 0 && node_idx+element_max <NODE_LIST_SIZE){
		//할당 가능.
		ret->node_start = node_idx;
		ret->node_max = node_idx+element_max;
		node_idx +=element_max;
	}else{
		return 0;
	}

	Uart_Printf("node_idx = %d, ret->node_start =%d, ret->node_max = %d\n",\
				node_idx, ret->node_start, ret->node_max);
	return ret;
}

int OS_Create_Task_Simple(void(*ptask)(void*), void* para, int prio, \
                          int size_stack, int q_element_size, int q_element_max)
{
	// ptask : task 코드의 시작주소
	// para : task코드가 시작하면서 전달받을 parameter
	// prio : 우선순위
	// size_stack : task가 사용할 stack의 할당 요청 사이즈

	static int idx_tcb = 0;
	TCB *ptcb;

	// task가 사용 할 TCB 할당
	if(idx_tcb >= MAX_TCB)
	{
		return OS_FAIL_ALLOCATE_TCB;
	}
	ptcb = &tcb[idx_tcb];

	// task가 사용 할 stack 할당
	ptcb->top_of_stack = (unsigned long*)_OS_Get_Stack(size_stack);
	if(ptcb->top_of_stack == (unsigned long*)0)
	{
		return OS_FAIL_ALLOCATE_STACK;
	}
	if( q_element_max !=0 && q_element_size !=0 ){
		ptcb->task_message_q = (Queue*)_OS_Get_Queue(q_element_size, q_element_max);
		Uart_Printf("idx_tcb = %d, ptcb->task_message_q = %x\n", idx_tcb, ptcb->task_message_q);
	}else{
		Uart_Printf("invalid param, q_element_max = %d, q_element_size = %d\n",\
		q_element_max, q_element_size);
		ptcb->task_message_q = (Queue*) 0;
	}

	idx_tcb++; // 다음 tcb 할당을 위한 index 이동

	// Task의 초기 context 정의
	ptcb->top_of_stack -= 16;
	ptcb->top_of_stack[8] = (unsigned long)para;
	ptcb->top_of_stack[14] = (unsigned long)ptask;
	ptcb->top_of_stack[15] = INIT_PSR;

	ptcb->prio = prio;
	ptcb->original_prio = prio;
	ptcb->blocked_mutex_id = -1;
	ptcb->state = TASK_STATE_READY;
	ptcb->next = 0;
	ptcb->wakeup_target_time = 0;
	ptcb->mutex_time=0;
    if (ptcb->prio < 0 || ptcb->prio >= MAX_PRIORITY)
        return 0;
    Enqueue(ready_Queues[ptcb->prio], ptcb, TCB_PTR);

	Uart_Printf("idx_tcb = %d, ptcb = %x\n", idx_tcb-1, ptcb);
	return ptcb->no_task;
}

extern void _OS_Start_First_Task(void); // scheduler.s 파일 확인

void OS_Scheduler_Start(void)
{
	int i;

	// 현재는 선택된 첫 task의 실행만 진행하고 있음 (임의로 tcb[0]의 task를 현재 실행 할 태스크로 정의 (추후 정책에 따른 선택의 코드로 변경 필요)
	current_tcb = &tcb[0];
	p_current_tcb = &current_tcb;

	// Exception Priority 초기화
	SCB->SHP[15 - 4] = 0xf << 4; // SysTick Exception Priority : Lowest Priority
	SCB->SHP[14 - 4] = 0xf << 4; // PendSV Exception Priority : Lowest Priority
	for(i = 0;i <= 42;i++)
	{
		NVIC_SetPriority(i, 0xe);
	}
	mutex_is_init = 1;
	SysTick_OS_Tick(SYSTICK);
	_OS_Start_First_Task();
}

// 다음 실행할 task 가져오기
TCB* _OS_Get_NextTask() {
	int i;
	for (i = 0; i < MAX_PRIORITY; i++) {
		
        while(!Is_Queue_Empty(ready_Queues[i])){
			Node* node = Dequeue(ready_Queues[i]);
			TCB* task = (TCB*)node->data;
			if (i == task->prio) {
				// Uart_Printf("scheduler : task->prio = %d\n", task->no_task);
				return task;
			}
        }
    }
    return 0; // 실행할 task가 없음
}

/*
param: void
return : void
description :
 determine the next_tcb.
 if there is no next_tcb, current_tcb will be executed repeatly.
*/
void _OS_Scheduler(void){
	
	TCB* task = 0;
	task = _OS_Get_NextTask();
	if (task == 0){
		next_tcb = &tcb[0];
	}else{
		next_tcb = task;
	}
	//return to PendSV_Handler
	return;
}

void _OS_Scheduler_Restore_Expired_TCB(void){
	int i;
	for(i=0; i< MAX_TCB; i++){
		//깨워야하는 애들
		if ((tcb[i].state==TASK_STATE_BLOCKED)  	&& \
			(tcb[i].wakeup_target_time != 0)		&& \
		   	(tcb[i].wakeup_target_time <= sys_cnt))
		{
			tcb[i].state = TASK_STATE_READY;
			tcb[i].wakeup_target_time =0;
			Enqueue(ready_Queues[tcb[i].prio], &tcb[i], TCB_PTR);
		}
	}
}

void _OS_Scheduler_Handle_Signaling_Flag(void){
	int i;
	int tcb_idx, data;
	Node* node;

	while(!Is_Queue_Empty(signaling_Queue)){
		node = Dequeue(signaling_Queue);
		tcb_idx = ((Signal_st*)node->data)->tcb_idx;
		data = ((Signal_st*)node->data)->data;

		Enqueue (tcb[tcb_idx].task_message_q, data,INT);
		switch(tcb[tcb_idx].state){
			case TASK_STATE_BLOCKED:
				tcb[tcb_idx].state = TASK_STATE_READY;
				Enqueue(ready_Queues[tcb[tcb_idx].prio], &tcb[tcb_idx], TCB_PTR);
				break;
		}
	}
	return;
}

void _OS_Scheduler_Before_Context_CB(TCB* task){
	_OS_Scheduler_Handle_Signaling_Flag();
	_OS_Scheduler_Restore_Expired_TCB();
	switch (task->state){
		case TASK_STATE_BLOCKED:
			break;
		case TASK_STATE_RUNNING:
			task->state = TASK_STATE_READY;
			Enqueue(ready_Queues[task->prio], task, TCB_PTR);
			break;
	}
	return;
}

void _OS_Scheduler_After_Context_CB(TCB* task){
	task->state = TASK_STATE_RUNNING;
	return;
}

/*
param : uint16_t time
return : void
description :
 - Task will be blocked during the paprameter time.
 - @TODO State change of task, Timer set for returing the state to the ready from blocked.
*/

void OS_Set_Task_Block(TCB* task, unsigned int block_time){
	volatile int i;
	// if user가 5s = 5000ms입력할 때,
	// 5000/SYSTICK, systick = 1000이면 cnt 5개 세주면 되고
	// systick = 1이면 5000을 세주면 됨.
	// 이거 처리하는데 몇 ms 걸릴까. 그거는 HW limitation으로 지원 불가할텐데.
	unsigned int convert_to_block_time_cnt = block_time/SYSTICK;
	unsigned int wakeup_time=0;
	if (sys_cnt+convert_to_block_time_cnt>SYS_CNT_MAX){
		wakeup_time = (sys_cnt+convert_to_block_time_cnt)%SYS_CNT_MAX;
	}else{
		wakeup_time = (sys_cnt+convert_to_block_time_cnt);
	}

	__disable_irq();
	task->state = TASK_STATE_BLOCKED;
	task->wakeup_target_time = wakeup_time;
	SCB->ICSR = (1<<SCB_ICSR_PENDSVSET_Pos);
	__enable_irq();
	
	return;
}

int OS_Create_Mutex(void) {
    int i;

    __disable_irq();
    for (i = 0; i < MAX_MUTEX; i++) {
        if (mutexs[i].used == 0) {
			mutexs[i].used = 1;
			mutexs[i].owner = -1;
            __enable_irq();
			Uart1_Printf(current_tcb, "OS_Create_Mutex %d", i);
            return i;
        }
    }
    __enable_irq();
    return -1;
}

int OS_Mutex_Lock(TCB* request_task, int mutex_idx) {
	// OS is not inited yet.
	if(mutex_is_init == 0){
		Uart1_Printf("OS_Mutex_Lock: start\n");
		return 1;
	}
		
    __disable_irq();
	// 1) no one does not use the mutex. 
	// 2) now == request  
	// -> can give the mutex.
    if (mutexs[mutex_idx].owner == -1 || mutexs[mutex_idx].owner == request_task->no_task) {
        mutexs[mutex_idx].owner = request_task->no_task;
        __enable_irq();
        return 1;
    } 
	else {
	/*
		priority inheritance
		 - priority check
		 - pop the task which has lower priority from ready queue.
		 - modify priority of the lower to the higher
		 - re-enqueue to the higher ready queue.
		 - change the state of request_task to STATE_BLOCKED and save the mutex_id
		 - Call scheduler
	*/
#ifdef FEATURE_PRIORITY_INHERITANCE
		if (tcb[mutexs[mutex_idx].owner].prio > request_task->prio) {
			tcb[mutexs[mutex_idx].owner].prio = request_task->prio;
			Enqueue(ready_Queues[tcb[mutexs[mutex_idx].owner].prio], &tcb[mutexs[mutex_idx].owner], TCB_PTR);
        }
#endif
		request_task->blocked_mutex_id = mutex_idx;
		request_task->state = TASK_STATE_BLOCKED;
		request_task->mutex_time = (unsigned int) sys_cnt;
		SCB->ICSR = (1<<SCB_ICSR_PENDSVSET_Pos);
        __enable_irq();
        return 0;
    }
}

void OS_Mutex_Unlock(TCB* request_task, int mutex_idx) {
	int i;
	unsigned int mutex_time_cmp =(unsigned int)0xffffffff;
	int wakeup_task_no =-1;
    __disable_irq();
	if(tcb[mutexs[mutex_idx].owner].prio != tcb[mutexs[mutex_idx].owner].original_prio){
		tcb[mutexs[mutex_idx].owner].prio = tcb[mutexs[mutex_idx].owner].original_prio;
	}
	
	for(i = 0; i< MAX_TCB; i++) {
		if ((tcb[i].state==TASK_STATE_BLOCKED) 		&& \
			(tcb[i].blocked_mutex_id == mutex_idx)  && \
			 tcb[i].wakeup_target_time == 0)
		{	
 			if((tcb[i].mutex_time != 0 ) 			&&\
			   (tcb[i].mutex_time < mutex_time_cmp))
			{
				wakeup_task_no = i;
				mutex_time_cmp = tcb[i].mutex_time;
			}
		}
	}
	if(wakeup_task_no != -1){
		tcb[wakeup_task_no].state = TASK_STATE_READY;
		tcb[wakeup_task_no].blocked_mutex_id = -1;
		tcb[wakeup_task_no].mutex_time = 0;
		mutexs[mutex_idx].owner =wakeup_task_no;
		Enqueue(ready_Queues[tcb[wakeup_task_no].prio], &tcb[wakeup_task_no], TCB_PTR);
	}else{
		mutexs[mutex_idx].owner = -1;
	}
    __enable_irq();
	return;
}


void Remove_Task_From_Ready_Queue(Queue* q, int no_task) {
	int i;
	Delete_Queue_node(q, no_task);
	return;
}
