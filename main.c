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
//	unsigned int cnt =0;
	for(;;)
	{

		Uart_Printf("Task1 start\n ");
		next_tcb = &tcb[1];
		SCB->ICSR = (1<<SCB_ICSR_PENDSVSET_Pos);
		for(i=0;i<0x100000;i++);
		if(systick_flag)
		{
			TCB* task = get_next_task();
			if (task != 0) {
				Uart_Printf("no_task: %d, priority: %d\n", task->no_task, task->prio);
			} else {
				Uart_Printf("No task to execute\n");
			}
			next_tcb = task;
			SCB->ICSR |= (1<<SCB_ICSR_PENDSVSET_Pos);
		}
	}
}

void Task2(void *para)
{
	volatile int i =0;
	for(;;){
		Uart_Printf("Task2 start\n ");
		next_tcb = &tcb[2];
		SCB->ICSR = (1<<SCB_ICSR_PENDSVSET_Pos);
		for(i=0;i<0x100000;i++);
		if(systick_flag)
		{
			TCB* task = get_next_task();
			if (task != 0) {
				Uart_Printf("no_task: %d, priority: %d\n", task->no_task, task->prio);
			} else {
				Uart_Printf("No task to execute\n");
			}
			next_tcb = task;
			SCB->ICSR |= (1<<SCB_ICSR_PENDSVSET_Pos);
		}
	}
	
}

void Task3(void *para)
{
	volatile int i;
	for(;;)
	{
		Uart_Printf("Task3 start\n");
		next_tcb = &tcb[0];
		SCB->ICSR = (1<<SCB_ICSR_PENDSVSET_Pos);
		for(i=0;i<0x100000;i++);
		if(systick_flag)
		{
			TCB* task = get_next_task();
			if (task != 0) {
				Uart_Printf("no_task: %d, priority: %d\n", task->no_task, task->prio);
			} else {
				Uart_Printf("No task to execute\n");
			}
			next_tcb = task;
			SCB->ICSR |= (1<<SCB_ICSR_PENDSVSET_Pos);
		}
	}
}


void Main(void)
{
	// ���� : Main �Լ��� ���� ������ ��� ���� ����
	// Thread mode
	// Thread mode�� ���� : Priviliged
	// Thread mode�� MSP ��� : ���� RTOS�� �ʱ�ȭ ������ ���� �����Ƿ� MSP ����ϸ� ����, ���� Task �ڵ� ���� �� PSP ������� ��ȯ
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