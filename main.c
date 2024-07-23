#include "device_driver.h"
#include "OS.h"
#include "queue.h"
#include "lcd_test.h"

extern TCB* next_tcb;
extern TCB* current_tcb;
extern TCB** p_current_tcb;
extern TCB tcb[MAX_TCB];
extern volatile int tim4_timeout;
extern volatile int systick_flag;
extern Queue ready_Queues[MAX_PRIORITY];
extern Queue* signaling_Queue;
extern int game_state_flag;
extern int plane_move_flag;
extern int level_delay[3];
extern int game_level;

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
			Uart_Printf("Dequeue data = %c\n", (int)node->data);
		}
		while(timeout>sys_cnt){
			// Uart_Printf("tcb[tcb_idx].task_message_q->element_cnt= %d\n",current_tcb->task_message_q->element_cnt);
			if(!Is_Queue_Empty(current_tcb->task_message_q)){
				node = Dequeue(current_tcb->task_message_q);
				timeout = sys_cnt+1000;
				Uart_Printf("in timeout Dequeue data = %c\n", (int)node->data);
			}else{
				LED_All_On();
				for(i=0;i<0x100000;i++);
				LED_All_Off();
				for(i=0;i<0x100000;i++);
			}
		}
		// Uart_Printf("Key task blocked!!\n");
		OS_Set_Task_Block(current_tcb, 5000);
	}
}

void Task_1_1(void *para) //Game Start Display
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

void Task_2_1(void *para) //Move Plane
{
	volatile int i;
	Node* node;
	unsigned int timeout;
	for(;;){
		if(game_state_flag != 1) continue;
		timeout = sys_cnt;
		while(!Is_Queue_Empty(current_tcb->task_message_q)){
			node = Dequeue(current_tcb->task_message_q);

			//task_message_q의 data 값이 **일 경우 동작
			if(node->data>=1 && node->data<=4)
				Game_Plane_Move(node->data);
			
			timeout = sys_cnt + 1000;
		}
		while(timeout>sys_cnt){
			// Uart_Printf("tcb[tcb_idx].task_message_q->element_cnt= %d\n",current_tcb->task_message_q->element_cnt);
			if(!Is_Queue_Empty(current_tcb->task_message_q)){
				node = Dequeue(current_tcb->task_message_q);
				timeout = sys_cnt+1000;
				//task_message_q의 data 값이 **일 경우 동작
				if(node->data>=1 && node->data<=4)
					Game_Plane_Move(node->data);
			}else{
				
			}
		}
		//Uart_Printf("Key task blocked!!\n");
		if(current_tcb->state !=TASK_STATE_BLOCKED)
			OS_Set_Task_Block(current_tcb, 100);
	}
}
void Task_2_2(void *para) //Generate Missile
{
	volatile int i =0;
	for(;;){
		if(game_state_flag != 1) continue;
		Game_Missile_Generation();
		if(current_tcb->state !=TASK_STATE_BLOCKED)
			OS_Set_Task_Block(current_tcb, 5000);
	}
	
}
void Task_2_3(void *para) //Move Missile
{
	for(;;)
	{
		if(game_state_flag != 1) continue;
		Game_Missile_Move();
		if(current_tcb->state !=TASK_STATE_BLOCKED)
			OS_Set_Task_Block(current_tcb, 1000);
	}
	
}

void Task_4_1(void *para) //Game Reset&Start
{
	volatile int i;
	Node* node;
	unsigned int timeout;

	for(;;)
	{
		timeout = sys_cnt;
		while(!Is_Queue_Empty(current_tcb->task_message_q)){
			node = Dequeue(current_tcb->task_message_q);

			//task_message_q의 data 값이 **일 경우 동작
			if(node->data==321) Game_Init();
			timeout = sys_cnt + 1000;
		}
		while(timeout>sys_cnt){
			// Uart_Printf("tcb[tcb_idx].task_message_q->element_cnt= %d\n",current_tcb->task_message_q->element_cnt);
			if(!Is_Queue_Empty(current_tcb->task_message_q)){
				node = Dequeue(current_tcb->task_message_q);
				timeout = sys_cnt+1000;
				if(node->data==321) Game_Init();
			}else{
				
			}
		}
		//Uart_Printf("Key task blocked!!\n");
		OS_Set_Task_Block(current_tcb, 100);
	}
	
}

void Task_4_2(void *para) //LCD Print
{
	volatile int i =0;
	for(;;){
		Draw_LCD();

		if(current_tcb->state !=TASK_STATE_BLOCKED)
			OS_Set_Task_Block(current_tcb, 100);
	}
	
}

void Task_4_3(void *para) //BGM Print
{
	volatile int i =0;
	for(;;){
		Play_BGM();

		if(current_tcb->state !=TASK_STATE_BLOCKED)
			OS_Set_Task_Block(current_tcb, 100);
	}
	
}


void Main(void)
{
	Uart_Printf("M3-Mini RTOS\n");
	
	OS_Init();	// OS Initialize
	Game_Init();

	game_state_flag = 0;

	OS_Create_Task_Simple(Task_2_1, (void*)0, 5, 1024, sizeof(int), 10); 
	OS_Create_Task_Simple(Task_2_2, (void*)0, 5, 1024, sizeof(int), 10); 
	OS_Create_Task_Simple(Task_2_3, (void*)0, 5, 1024, sizeof(int), 10); 
	OS_Create_Task_Simple(Task_4_1, (void*)0, 5, 1024, sizeof(int), 10); 
	OS_Create_Task_Simple(Task_4_2, (void*)0, 5, 1024, sizeof(int), 10); 

	OS_Scheduler_Start();	// Scheduler Start (������ ù��° Task�� ���ุ �ϰ� ����)

	for(;;)
	{

	}
}
