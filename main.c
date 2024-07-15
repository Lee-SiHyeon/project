#include "device_driver.h"
#include "OS.h"

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
		Uart_Printf("Task1 set next_tcb = %x\n", &tcb[1]);
		 next_tcb = &tcb[1];
		 Uart_Printf("D1 Task1 p_current_tcb = %x, current_tcb = %x\n", *p_current_tcb, current_tcb);
		 SCB->ICSR |= (1<<SCB_ICSR_PENDSVSET_Pos);
		 Uart_Printf("D2 Task1 p_current_tcb = %x\n", *p_current_tcb);
		 for(i=0;i<0x100000;i++);
//		cnt++;
//		if(tim4_timeout){
//
//			Uart_Printf("Tim4 timeout, cnt = %u\n",cnt);
//			tim4_timeout=0;
//		}
//		if (systick_flag){
//			Uart_Printf("systick timeout, cnt = %u\n",cnt);
//			systick_flag =0;
//		}
//		if(cnt >10000000 ){
//			cnt=0;
//		}


	}
}

void Task2(void *para)
{
	volatile int i =0;
	for(;;){
		Uart_Printf("Task2 start\n ");
		Uart_Printf("Task2 set next_tcb = %x\n", &tcb[0]);

		next_tcb = &tcb[0];
//		current_tcb = &tcb[1];
		Uart_Printf("D1 Task2 p_current_tcb = %x, current_tcb = %x\n", *p_current_tcb, current_tcb);

		SCB->ICSR |= (1<<SCB_ICSR_PENDSVSET_Pos);
		Uart_Printf("D2 Task2 p_current_tcb = %x, current_tcb = %x\n", *p_current_tcb, current_tcb );
		Uart_Printf("Task2 done\n ");
		for(i=0;i<0x100000;i++);
	}
	
}

void Task3(void *para)
{
	volatile int i;
	int cnt = 0;
	for(;;)
	{
		Uart_Printf("Task3 : %d\n", cnt++);
		for(i=0;i<0x100000;i++);
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

	OS_Create_Task_Simple(Task1, (void*)0, 5, 1024); // Task 占쏙옙占쏙옙
	OS_Create_Task_Simple(Task2, (void*)0, 5, 1024); // Task 占쏙옙占쏙옙
	Uart_Printf("Task1 = %x\n", &tcb[0]);
	Uart_Printf("Task2 = %x\n", &tcb[1]);

	OS_Scheduler_Start();	// Scheduler Start (������ ù��° Task�� ���ุ �ϰ� ����)

	for(;;)
	{

	}
}
