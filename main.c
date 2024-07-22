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
		if(current_tcb->state != TASK_STATE_BLOCKED){
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
			Uart_Printf("Key task blocked!!\n");
			OS_Set_Task_Block(current_tcb, 5000);
		}else{
			Uart_Printf("this task is blocked\n");
		}

	}
}

void Game_Start_Task(void *para){
	for(;;) {

	}
}

void Airplane_Control_Task(void *para) {
    for(;;) {
        Update_Airplane_Position();
        OS_Set_Task_Block(current_tcb, 50);
    }
}

void Bullet_Management_Task(void *para) {
    for(;;) {
        if (Player_Shoots()) {
            Generate_Bullet();
		}
        OS_Set_Task_Block(current_tcb, 50);
    }
}

void Missile_Management_Task(void *para) {
	// 조건
		Generate_Missile();
    for(;;) {
		Move_Missiles();
        OS_Set_Task_Block(current_tcb, 5000);
    }
}

void Collision_Detection_Task(void *para) {
    for(;;) {
		// 비행기가 미사일 맞았을 때
        if (Detect_Collision()) {
            Game_Over();
            break;
        }

		// 비행기가 쏜 미사일에 미사일이 맞았을 때
		if(Missile_Hit_Detection()){
			Remove_Missile();
			Remove_Bullet();
		}
		OS_Set_Task_Block(current_tcb, 50);
    }
}

void Buzzer_Control_Task(void *para) {
    for(;;) {
        Control_Buzzer();
		OS_Set_Task_Block(current_tcb, 50);
    }
}

void LDC_Display_Task(void *para) {
    for(;;) {
		for() // 비행기, 총알, 미사일, 점수가 달라졌으면 그린다.
		{
			Draw_Object(&object);
		}
        OS_Set_Task_Block(current_tcb, 10);
    }
}

void Main(void)
{
	Uart_Printf("M3-Mini RTOS\n");
	OS_Init();	// OS �ڷᱸ�� �ʱ�ȭ

	OS_Create_Task_Simple(Key_Receive_Task, (void*)0, 3, 2048,sizeof(Node)+4, 10); 
    OS_Create_Task_Simple(Airplane_Control_Task, (void*)0, 1, ...);
    OS_Create_Task_Simple(Bullet_Management_Task, (void*)0, 3);
    OS_Create_Task_Simple(Missile_Management_Task, (void*)0, 5);
    OS_Create_Task_Simple(Collision_Detection_Task, (void*)0, 3);
    OS_Create_Task_Simple(Buzzer_Control_Task, (void*)0, 5);
    OS_Create_Task_Simple(LDC_Display_Task, (void*)0, 5);
	
	OS_Scheduler_Start();

	for(;;)
	{

	}
}
