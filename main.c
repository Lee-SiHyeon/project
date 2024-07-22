#include "device_driver.h"
#include "OS.h"
#include "queue.h"

extern TCB* next_tcb;
extern TCB* current_tcb;
extern TCB** p_current_tcb;
extern TCB tcb[MAX_TCB];
extern volatile int tim4_timeout;
extern volatile int systick_flag;
extern Queue ready_Queues[MAX_PRIORITY];
extern Queue* signaling_Queue;
void Task0(void *para){
	for(;;)
	{
		// Uart_Printf(current_tcb, "Task 0 start\n");
	}
}

void Task1(void *para)
{
	volatile int i =0;
	static Signal_st data;
	char action_flag=0;
	for(;;)
	{
		LED_0_Only_On();
		for(i=0;i<0x100000;i++);
		LED_0_Only_Off();
		for(i=0;i<0x100000;i++);
		if(current_tcb->state !=TASK_STATE_BLOCKED) 
			OS_Set_Task_Block(current_tcb, 500);
	}
}

void Task2(void *para)
{
	volatile int i =0;
	for(;;){
		LED_1_Only_On();
		for(i=0;i<0x100000;i++);
		LED_1_Only_Off();
		for(i=0;i<0x100000;i++);
		if(current_tcb->state !=TASK_STATE_BLOCKED) 
			OS_Set_Task_Block(current_tcb, 300);
	}
	
}

/*
sw0: EXTI15_10_IRQHandler (kv: 1 key_value: 5)

sw1: EXTI15_10_IRQHandler (kv: 2 key_value: 6)

(선 연결된 쪽이 상)

조이스틱 상: EXTI3_IRQHandler (key_value: 1)

조이스틱 하: EXTI9_5_IRQHandler (kv: 1 key_value: 2)

조이스틱 좌: EXTI9_5_IRQHandler (kv: 2 key_value: 3)

조이스틱 우: EXTI9_5_IRQHandler (kv: 4 key_value: 4)

조이스틱 push: EXTI15_10_IRQHandler (kv: 1 key_value: 5)

key0: EXTI9_5_IRQHandler (kv: 2 key_value: 3)

key1: EXTI9_5_IRQHandler (kv: 4 key_value: 4)
*/
extern unsigned int sys_cnt;
void Key_Receive_Task(void *para)
{
	volatile int i;
	Node* node;
	unsigned int timeout;
	for(;;)
	{
		timeout = sys_cnt;
		while(!Is_Queue_Empty(current_tcb->task_message_q)){
			node = Dequeue(current_tcb->task_message_q);
			timeout = sys_cnt + 1000;
			Uart_Printf(current_tcb, "Dequeue data = %c\n", (int)node->data);
		}
		while(timeout>sys_cnt){
			// Uart_Printf(current_tcb, "tcb[tcb_idx].task_message_q->element_cnt= %d\n",current_tcb->task_message_q->element_cnt);
			if(!Is_Queue_Empty(current_tcb->task_message_q)){
				node = Dequeue(current_tcb->task_message_q);
				timeout = sys_cnt+1000;
				Uart_Printf(current_tcb, "in timeout Dequeue data = %c\n", (int)node->data);
			}else{
				LED_All_On();
				for(i=0;i<0x100000;i++);
				LED_All_Off();
				for(i=0;i<0x100000;i++);
			}
		}
		// Uart_Printf(current_tcb, "Key task blocked!!\n");
		OS_Set_Task_Block(current_tcb, 5000);
	}
}


void Main(void)
{
	// Uart_Printf("M3-Mini RTOS\n");
	OS_Init();	// OS �ڷᱸ�� �ʱ�ȭ
	
	OS_Create_Task_Simple(Task0, (void*)0, 6, 2048,4,10);
	OS_Create_Task_Simple(Task1, (void*)0, 3, 2048,4,10);
	OS_Create_Task_Simple(Task2, (void*)0, 2, 2048,4,10); 
	OS_Create_Task_Simple(Key_Receive_Task, (void*)0, 1, 2048,sizeof(int), 10); 
	
	OS_Scheduler_Start();	// Scheduler Start (������ ù��° Task�� ���ุ �ϰ� ����)

	for(;;)
	{

	}
}
