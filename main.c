#include "device_driver.h"
#include "OS.h"
#include "queue.h"

extern TCB* next_tcb;
extern TCB* current_tcb;
extern TCB** p_current_tcb;
extern TCB tcb[MAX_TCB];
extern volatile int tim4_timeout;
extern volatile int systick_flag;
void Task1(void *para)
{
	volatile int i =0;
	for(;;)
	{
		LED_0_Only_On();
		Uart_Printf("Task1 start, task current state =%d\n", current_tcb->state);
		for(i=0;i<0x100000;i++);
		LED_0_Only_Off();
		for(i=0;i<0x100000;i++);
	}
}

void Task2(void *para)
{
	volatile int i =0;
	for(;;){
		LED_1_Only_On();
		Uart_Printf("Task2 start, task current state =%d\n", current_tcb->state);
		for(i=0;i<0x100000;i++);
		LED_1_Only_Off();
		for(i=0;i<0x100000;i++);
	}
	
}

void Task3(void *para)
{
	volatile int i;
	for(;;)
	{
		Uart_Printf("Task3 start, task current state =\n", current_tcb->state);
		LED_All_On(); // LED 0, 1 on.
		for(i=0;i<0x100000;i++);
		LED_All_Off(); // LED 0, 1 on.
		for(i=0;i<0x100000;i++);
	}
}


void Main(void)
{
	Uart_Printf("M3-Mini RTOS\n");

	OS_Init();	// OS �ڷᱸ�� �ʱ�ȭ
	
	OS_Create_Task_Simple(Task1, (void*)0, 5, 1024); 
	OS_Create_Task_Simple(Task2, (void*)0, 5, 1024); 
	OS_Create_Task_Simple(Task3, (void*)0, 5, 1024); 

	OS_Scheduler_Start();	// Scheduler Start (������ ù��° Task�� ���ุ �ϰ� ����)

	for(;;)
	{

	}
}
