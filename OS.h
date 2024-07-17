#ifndef _OS_H_
#define _OS_H_

/* [Constant] */
#define MAX_TCB					(20)
#define MAX_PRIORITY 			(10)
#define MAX_TASKS 				(100)
#define BLOCK_LIST_SIZE			(5)
#define SYSTICK					(1000)
#define SYS_CNT_MAX				(UINT32_MAX / SYSTICK)

#define PRIO_HIGHEST			(0)
#define PRIO_LOWEST				(10)
#define NUM_PRIO				(PRIO_LOWEST - PRIO_HIGHEST + 1)

#define OS_SUCCESS				(0)
#define OS_FAIL_ALLOCATE_TCB	(-1)
#define OS_FAIL_ALLOCATE_STACK	(-2)

#define STACK_SIZE				(8 * 1024)
typedef enum {
	TASK_STATE_NONE =0,
	TASK_STATE_READY,
	TASK_STATE_BLOCKED,
	TASK_STATE_RUNNING,
	TASK_STATE_MAX,
}Task_State;

#define INIT_PSR				(0x01000000)

/* [ Type ] */

typedef struct _tcb{
	unsigned long* top_of_stack;	// task ������� stack�� top
	int no_task;					// task ��ȣ (tcb �迭�� index)
	int prio;						// task�� priority
	int state;						// task�� ����
	unsigned int wakeup_target_time;
	unsigned int systick_cnt_at_blocked; // for debug
	struct _tcb* next;
}TCB;

/* [ Macro ] */

/* [ Function ] */
extern void OS_Init(void);
extern int OS_Create_Task_Simple(void(*ptask)(void*), void* para, int prio, int size_stack);
extern void OS_Scheduler_Start(void);
void OS_Set_Task_Block(TCB* task, unsigned int block_time);
TCB* _OS_Get_NextTask();
void _OS_Init_Scheduler();
#endif
