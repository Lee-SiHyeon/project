#include "device_driver.h"

char * _sbrk(int inc)
{
	extern unsigned char __ZI_LIMIT__;
	static char * heap = (char *)0;

	char * prevHeap;
	char * nextHeap;

	if(heap == (char *)0) heap = (char *)HEAP_BASE;

	prevHeap = heap;
	nextHeap = (char *)((((unsigned int)heap + inc) + 0x7) & ~0x7);

	if((unsigned int)nextHeap >= HEAP_LIMIT) return (char *)0;

	heap = nextHeap;
	return prevHeap;
}

void System_Init(void)
{
	// Device �ʱ�ȭ
	Uart1_Init(115200); // Uart1 ��ġ �ʱ�ȭ
	Uart1_RX_Interrupt_Enable(1); // Uart1 RX �̺�Ʈ interrupt Ȱ��ȭ
	Key_Poll_Init();	// Key(LCD ������ Ű) �ʱ�ȭ
	Key_ISR_Enable(1);	// Key push �̺�Ʈ  interrupt Ȱ��ȭ
	Lcd_Init();	// LCD �ʱ�ȭ
	LED_Init();	// LED �ʱ�ȭ
	TIM3_Out_Init();
	
	SCB->VTOR = 0x08003000;
	SCB->SHCSR = 7<<16;
	SCB->AIRCR = 0x05FA0000;
}
