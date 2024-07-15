#include "device_driver.h"

#define TIM4_TICK	  		(20) 				// usec
#define TIM4_PRESCALER 	  		7200 // Hz
#define TIME4_PLS_OF_1ms  	(1000/TIM4_TICK)
#define TIM4_MAX	  		(0xffffu)

void TIM4_Repeat_Interrupt_Enable(int en, int time)
{
    if(en)
    {
        Macro_Set_Bit(RCC->APB1ENR, 2);

        TIM4->CR1 = (1<<4)|(0<<3);
        TIM4->PSC = TIM4_PRESCALER-1;
        TIM4->ARR = 10* time;
        Macro_Set_Bit(TIM4->EGR,0);

        // TIM4->SR �������Ϳ��� Timer Pending Clear
        Macro_Clear_Bit(TIM4->SR, 0);

        // NVIC���� 30�� ���ͷ�Ʈ Pending Clear => NVIC�� Macro ���
        NVIC_ClearPendingIRQ(TIM4_IRQn);

        // TIM4->DIER �������Ϳ��� Timer ���ͷ�Ʈ ���
        Macro_Set_Bit(TIM4->DIER, 0);

        // NVIC���� 30�� ���ͷ�Ʈ�� ������� ���� => NVIC�� Macro ���
        NVIC_EnableIRQ(TIM4_IRQn);

        // TIM4 Start
        Macro_Set_Bit(TIM4->CR1, 0);
    }

    else
    {
        NVIC_DisableIRQ(TIM4_IRQn);
        Macro_Clear_Bit(TIM4->CR1, 0);
        Macro_Clear_Bit(TIM4->DIER, 0);
    }
}
