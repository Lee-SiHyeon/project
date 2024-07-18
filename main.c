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
		Uart_Printf("Task 0 start\n");
	}
}

void Task1(void *para)
{
	volatile int i =0;
	for(;;)
	{
		Uart_Printf("Task 1 start\n");
		LED_0_Only_On();
		for(i=0;i<0x100000;i++);
		LED_0_Only_Off();
		for(i=0;i<0x100000;i++);
		// if(current_tcb->state !=TASK_STATE_BLOCKED) 
			// OS_Set_Task_Block(current_tcb, 500);
	}
}

void Task2(void *para)
{
	volatile int i =0;
	for(;;){
		Uart_Printf("Task 2 start\n");
		LED_1_Only_On();
		for(i=0;i<0x100000;i++);
		LED_1_Only_Off();
		for(i=0;i<0x100000;i++);
		// if(current_tcb->state !=TASK_STATE_BLOCKED) 
			// OS_Set_Task_Block(current_tcb, 300);
	}
	
}

void Task3(void *para)
{
	volatile int i;
	for(;;)
	{
		Uart_Printf("Task 3 start\n");
		LED_All_On(); // LED 0, 1 on.
		for(i=0;i<0x100000;i++);
		LED_All_Off(); // LED 0, 1 on.
		for(i=0;i<0x100000;i++);
		// if(current_tcb->state !=TASK_STATE_BLOCKED) 
			// OS_Set_Task_Block(current_tcb, 100);
	}
}


void Main(void)
{
	Uart_Printf("M3-Mini RTOS\n");
	OS_Init();	// OS �ڷᱸ�� �ʱ�ȭ
	
	OS_Create_Task_Simple(Task0, (void*)0, 6, 1024);
	OS_Create_Task_Simple(Task1, (void*)0, 1, 1024);
	OS_Create_Task_Simple(Task2, (void*)0, 1, 1024); 
	OS_Create_Task_Simple(Task3, (void*)0, 1, 1024); 

	OS_Scheduler_Start();	// Scheduler Start (������ ù��° Task�� ���ุ �ϰ� ����)

	for(;;)
	{

	}
}
