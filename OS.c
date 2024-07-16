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

Queue priorityQueues[MAX_PRIORITY];
/* Function */
void OS_Init(void)
{

	_OS_Init_Scheduler();

	int i;
	for(i=0; i<MAX_TCB; i++)
	{
		tcb[i].no_task = i;
	}
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
    if (ptcb->prio < 0 || ptcb->prio >= MAX_PRIORITY)
        return 0;
    Enqueue(&priorityQueues[ptcb->prio], ptcb);


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
	SysTick_OS_Tick(1000);
	_OS_Start_First_Task();
}

// 다음 실행할 task 가져오기
TCB* _OS_Get_NextTask() {
	int i;
	for (i = 0; i < MAX_PRIORITY; i++) {
        if (!Is_Queue_Empty(&priorityQueues[i])) {
			TCB* initTask = Dequeue(&priorityQueues[i]);
            TCB* task = initTask;
			do{
                if (task->state == TASK_STATE_READY) {
                    Enqueue(&priorityQueues[i], task); // 동일한 우선순위의 맨 끝으로 이동
                    return task;
                }
                Enqueue(&priorityQueues[i], task); // 다시 큐에 넣어줌
                task = Dequeue(&priorityQueues[i]);
            } while(initTask != task);
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
		next_tcb = current_tcb;
	}else{
		next_tcb = task;
	}
	//return to PendSV_Handler
	return;
}

void _OS_Scheduler_Before_Context_CB(TCB* task){
	task->state = TASK_STATE_READY;
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
void OS_Set_Task_Block(uint16_t time){

}
