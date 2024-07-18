#include "device_driver.h"
#include "OS.h"
#include "queue.h"

extern TCB* next_tcb;
extern TCB* current_tcb;
extern TCB** p_current_tcb;
extern TCB tcb[MAX_TCB];
extern volatile int tim4_timeout;
extern volatile int systick_flag;
extern Queue priorityQueues[MAX_PRIORITY];
extern TCB* blocked_task_list[BLOCK_LIST_SIZE];

void Task0(void *para){
	for(;;)
	{
		// Uart_Printf("Task 0 start\n");
	}
}

void Task1(void *para)
{
	volatile int i =0;
	for(;;)
	{
		// Uart_Printf("Task 1 start\n");
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
		// Uart_Printf("Task 2 start\n");
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

void Key_Receive_Task(void *para)
{
	volatile int i;
	Node* node;
	char timercall_flag = 0;
	current_tcb->event_wait_flag = 1;
	for(;;)
	{
		if(!Is_Queue_Empty(current_tcb->task_message_q)){
			node = Dequeue(current_tcb->task_message_q);
			Uart_Printf("Dequeue data = %c\n", (int)node->data);
		}
		//queue is empty
		else{
			LED_All_On();
			for(i=0;i<0x100000;i++);
			LED_All_Off();
			for(i=0;i<0x100000;i++);
			// //wait time.
			// if (timercall_flag ==0){
			// 	TIM4_Repeat_Interrupt_Enable(ENABLE, 1000);
			// 	timercall_flag =1;
			// }
			// while(!tim4_timeout){
			// 	if(!Is_Queue_Empty(current_tcb->task_message_q)){
			// 		node = Dequeue(current_tcb->task_message_q);
			// 		Uart_Printf("Dequeue data = %d\n", node->data);
			// 	}	
			// }
			// timercall_flag =0;
		}	
		Uart_Printf("Key task blocked!!\n");
		OS_Set_Task_Block(current_tcb, 5000);
	}
}


void Main(void)
{
	Uart_Printf("M3-Mini RTOS\n");
	OS_Init();	// OS �ڷᱸ�� �ʱ�ȭ
	
	OS_Create_Task_Simple(Task0, (void*)0, 6, 2048);
	OS_Create_Task_Simple(Task1, (void*)0, 1, 2048);
	OS_Create_Task_Simple(Task2, (void*)0, 2, 2048); 
	OS_Create_Task_Simple(Key_Receive_Task, (void*)0, 3, 2048); 

	OS_Scheduler_Start();	// Scheduler Start (������ ù��° Task�� ���ุ �ϰ� ����)

	for(;;)
	{

	}
}
