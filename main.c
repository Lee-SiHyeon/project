#include "device_driver.h"
#include "OS.h"
#include "queue.h"
#include "game.h"

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
extern Mutex mutexs[MAX_MUTEX];
volatile int no_mutex;
Signal_st wake_task_lcd;

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

void Task_Idle(void*para){
	for(;;){

	}
}
void Task_Key_Handle(void *para) //Move Plane
{
	volatile int i;
	Node* node;
	int key_value;
	unsigned int timeout;

	for(;;){

		while(!Is_Queue_Empty(current_tcb->task_message_q)){	
			node = Dequeue(current_tcb->task_message_q);
			key_value = (int)node->data;
			switch(key_value){
				case KEY_PLANE_FORWARD:
				case KEY_PLANE_BACK:
				case KEY_PLANE_LEFT:
				case KEY_PLANE_RIGHT:
					Game_Plane_Move(node->data);
					break;
				case KEY_PLANE_BULLET:
					Game_Bullet_Generation();
					break;
				case KEY_GAME_RESET:
					Game_Init();
					game_state_flag = GAME_PLAYING;
					break;
				default:
					break;
			}
			
			wake_task_lcd.data = -1;
			wake_task_lcd.tcb_idx = 2;
			Enqueue(signaling_Queue,(void*)&wake_task_lcd,STRUCT_SIGNAL);
			
			timeout = sys_cnt + 10;
		}
		while(timeout>sys_cnt){
			// Uart_Printf("tcb[tcb_idx].task_message_q->element_cnt= %d\n",current_tcb->task_message_q->element_cnt);
			if(!Is_Queue_Empty(current_tcb->task_message_q)){
				node = Dequeue(current_tcb->task_message_q);
				timeout = sys_cnt+10;
				//task_message_q의 data 값이 **일 경우 동작
				switch(key_value){
					case KEY_PLANE_FORWARD:
					case KEY_PLANE_BACK:
					case KEY_PLANE_LEFT:
					case KEY_PLANE_RIGHT:
						Game_Plane_Move(node->data);
						break;
					case KEY_PLANE_BULLET:
						Game_Bullet_Generation();
						break;
					case KEY_GAME_RESET:
						Game_Init();
						game_state_flag = GAME_PLAYING;
						break;
					default:
						break;
				}

				wake_task_lcd.data = -1;
				wake_task_lcd.tcb_idx = 2;
				Enqueue(signaling_Queue,(void*)&wake_task_lcd,STRUCT_SIGNAL);
				
			}else{
				
			}
		}
		if(current_tcb->state !=TASK_STATE_BLOCKED)
			OS_Set_Task_Block(current_tcb, 100);
	}
}

void Task_Generate_Missile(void *para) //Generate Missile
{
	
	volatile int i =0;
	for(;;){
		if(game_state_flag != GAME_PLAYING) continue;
		Game_Missile_Generation();
		if(current_tcb->state !=TASK_STATE_BLOCKED)
			OS_Set_Task_Block(current_tcb, 5000);
	}
	
}

void Task_Draw_LCD(void *para)
{
	volatile int i =0;
	unsigned int timeout;
	for(;;){
		Draw_LCD();

		if(current_tcb->state !=TASK_STATE_BLOCKED)
			OS_Set_Task_Block(current_tcb, 100);
	}
	
}

// void Task_4_3(void *para) //BGM Print
// {
// 	volatile int i =0;
// 	for(;;){
// 		Play_BGM();
// 		if(current_tcb->state !=TASK_STATE_BLOCKED)
// 			OS_Set_Task_Block(current_tcb, 100);
// 	}
// }

void Main(void)
{
	
	OS_Init();	// OS Initialize
	Game_Init();

	OS_Create_Task_Simple(Task_Idle, (void*)0, 4, 128, sizeof(Node), 1); // Move Plane 

	OS_Create_Task_Simple(Task_Key_Handle, (void*)0,0, 1024, sizeof(Node), 10); // Move Plane
	OS_Create_Task_Simple(Task_Draw_LCD, (void*)0, 3, 1024, sizeof(Node), 5); // LCD Print
	OS_Create_Task_Simple(Task_Generate_Missile, (void*)0, 3, 1024, sizeof(Node), 10); // Generate Missile //erase


	OS_Scheduler_Start();	// Scheduler Start (������ ù��° Task�� ���ุ �ϰ� ����)

	for(;;)
	{

	}
}
