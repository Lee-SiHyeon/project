#include "device_driver.h"
#include "OS.h"
#include "queue.h"

// 참고 : 최초 제공된 코드는 완전한 코드가 아님
//      그러므로 추후 RTOS 설계에 따라 보완이 필요함


/* Global Variable */
TCB tcb[MAX_TCB];
char stack[STACK_SIZE] __attribute__((__aligned__(8)));
volatile TCB* current_tcb;
volatile TCB* next_tcb;
volatile TCB** p_current_tcb;

TCB* blocked_task_list[BLOCK_LIST_SIZE];
char blocked_task_cnt = 0;
Queue priorityQueues[MAX_PRIORITY];

extern volatile unsigned int sys_cnt;

/* Function Prototype */
void _OS_Blocked_Task_init(void);

void OS_Init(void)
{
	_OS_Init_Scheduler();
	_OS_Blocked_Task_init();
	int i;
	for(i=0; i<MAX_TCB; i++)
	{
		tcb[i].no_task = i;
	}
	Init_Node_List();
}

// 스케줄러 초기화
void _OS_Init_Scheduler() {
    int i;
	for (i = 0; i < MAX_PRIORITY; i++) {
        Init_Queue(&priorityQueues[i]);
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

int OS_Create_Task_Simple(void(*ptask)(void*), void* para, int prio, int size_stack)
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

	idx_tcb++; // 다음 tcb 할당을 위한 index 이동

	// Task의 초기 context 정의
	ptcb->top_of_stack -= 16;
	ptcb->top_of_stack[8] = (unsigned long)para;
	ptcb->top_of_stack[14] = (unsigned long)ptask;
	ptcb->top_of_stack[15] = INIT_PSR;

	ptcb->prio = prio;
	ptcb->state = TASK_STATE_READY;
	ptcb->next = 0;
	ptcb->wakeup_target_time = 0;
	ptcb->systick_cnt_at_blocked = 0;

    if (ptcb->prio < 0 || ptcb->prio >= MAX_PRIORITY)
        return 0;
    Enqueue(&priorityQueues[ptcb->prio], ptcb, TCB_PTR);

	Uart_Printf("idx_tcb = %d, ptcb = %x\n", idx_tcb, ptcb);
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
	SysTick_OS_Tick(SYSTICK);
	_OS_Start_First_Task();
}

// 다음 실행할 task 가져오기
TCB* _OS_Get_NextTask() {
	int i;
	for (i = 0; i < MAX_PRIORITY; i++) {
        if (!Is_Queue_Empty(&priorityQueues[i])) {
			Node* node = Dequeue(&priorityQueues[i]);
			TCB* task = (TCB*)node->data;

            return task;
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
	for(i=0; i< BLOCK_LIST_SIZE; i++){
		//깨워야하는 애들
		if ((blocked_task_list[i]->state==TASK_STATE_BLOCKED) && \
		   	(blocked_task_list[i]->wakeup_target_time <= sys_cnt))
		{
			blocked_task_list[i]->state = TASK_STATE_READY;
			Enqueue(&priorityQueues[blocked_task_list[i]->prio], blocked_task_list[i], TCB_PTR);
			blocked_task_list[i] = 0;
			blocked_task_cnt--;
		}
	}
}

void _OS_Scheduler_Before_Context_CB(TCB* task){

	_OS_Scheduler_Restore_Expired_TCB();
	switch (task->state){
		case TASK_STATE_BLOCKED:
			break;
		case TASK_STATE_RUNNING:
			task->state = TASK_STATE_READY;
			Enqueue(&priorityQueues[task->prio], task, TCB_PTR);
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
	if (sys_cnt+convert_to_block_time_cnt>SYS_CNT_MAX){
		task->wakeup_target_time = (sys_cnt+convert_to_block_time_cnt)%SYS_CNT_MAX;
	}else{
		task->wakeup_target_time = (sys_cnt+convert_to_block_time_cnt);
	}

	task->systick_cnt_at_blocked = sys_cnt;
	for(i = 0; i<BLOCK_LIST_SIZE; i++){
		if(blocked_task_list[i]==0){
			__disable_irq();
			task->state = TASK_STATE_BLOCKED;
			blocked_task_list[i] = task;
			blocked_task_cnt++;
			__enable_irq();
			break;
		}
	}

	SCB->ICSR = (1<<SCB_ICSR_PENDSVSET_Pos);
	return;
}

void _OS_Blocked_Task_init(void){
	int i;
	for(i=0; i< BLOCK_LIST_SIZE; i++)
	{
		blocked_task_list[i] = 0;
	}
	return;
}