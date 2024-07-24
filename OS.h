#ifndef _OS_H_
#define _OS_H_

#include "queue.h"
/* [Constant] */
#define MAX_TCB					(20)
#define MAX_PRIORITY 			(5)
#define MAX_TASKS 				(100)
#define MAX_MUTEX 				(5)
#define BLOCK_LIST_SIZE			(5)
#define SYSTICK					(1)
#define SYS_CNT_MAX				(UINT32_MAX / SYSTICK)

#define PRIO_HIGHEST			(0)
#define PRIO_LOWEST				(MAX_PRIORITY)
#define NUM_PRIO				(PRIO_LOWEST - PRIO_HIGHEST + 1)

#define OS_SUCCESS				(0)
#define OS_FAIL_ALLOCATE_TCB	(-1)
#define OS_FAIL_ALLOCATE_STACK	(-2)
#define OS_FAIL_ALLOCATE_QUEUE  (-3)

#define STACK_SIZE				(8 * 1024)
#define QUEUE_SIZE				(4 * 1024)

#define INIT_PSR				(0x01000000)

#define FEATURE_PRIORITY_INHERITANCE

typedef enum {
	TASK_STATE_NONE =0,
	TASK_STATE_READY,
	TASK_STATE_BLOCKED,
	TASK_STATE_RUNNING,
	TASK_STATE_MAX,
}Task_State;

typedef enum {
	NONE = 0,
	Printf,
}Blocked_Reason;

typedef enum{
	WR_MIN,
	WR_BY_SET_BLOCK_TIMER,
	WR_BY_SIGNALING,
	WR_BY_MUTEX,
	WR_MAX
}Wakeup_Reason;
/* [ Type ] */
typedef struct _signal{
	int tcb_idx;
	int data;
}Signal_st;
typedef struct _tcb{
	unsigned long* top_of_stack;		
	int no_task;					
	int prio;
	int original_prio;		
	int state;						
	unsigned int wakeup_target_time;
	struct _tcb* next;
	Queue* task_message_q;
	int blocked_mutex_id;
	unsigned int mutex_time;
	char wakeup_reason;
}TCB;

typedef struct {
    int owner;
    int used;
} Mutex;

/* [ Macro ] */

/* [ Function ] */
extern void OS_Init(void);
int OS_Create_Task_Simple(void(*ptask)(void*), void* para, int prio, \
                          int size_stack, int q_element_size, int q_element_max);
Queue* _OS_Get_Queue(int element_size, int element_max);
extern void OS_Scheduler_Start(void);
void OS_Set_Task_Block(TCB* task, unsigned int block_time);
TCB* _OS_Get_NextTask();
void _OS_Init_Scheduler();
int OS_Mutex_Lock(TCB* tcb, int idx);
void OS_Mutex_Unlock(TCB* tcb, int idx);
void Remove_Task_From_Ready_Queue(Queue* queue, int task_id);
#endif