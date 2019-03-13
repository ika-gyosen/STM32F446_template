//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// タイマー関係
// 2019.2.3 sakai created.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef TIMER_446_C
#define TIMER_446_C

#include "Timer_446.h"

// =====================================================================================================================
//  TIM2の初期化と割り込み有効
// =====================================================================================================================
void TIM2_init() {
    // TIM2 clock enable on APB1
    RCC-> APB1ENR |= RCC_APB1ENR_TIM2EN;
    
    // timer counter settings
    TIM2->PSC = 45; // APB1 / PSC  --- 45000000 / 45 = 1000000 -> 1MHz
    TIM2->ARR = 1;  // PSC / ARR
    TIM2->CNT = 0;  // ARR / CNT
    
    // timer enable
    TIM2->CR1 |= TIM_CR1_CEN;

    // IRQ enable
    TIM2->DIER = TIM_DIER_UIE;

    // IRQ callback TIM2_IRQHandler()
    NVIC_SetPriority(TIM2_IRQn, 0);
    NVIC_EnableIRQ(TIM2_IRQn);
}

// =====================================================================================================================
// TIM2 Ticekr割り込まれ関数
// =====================================================================================================================
void TIM2_IRQHandler(void) {
    /* Your Code */
    
    static int flag = 0;
    
    flag =! flag;
    
    if( flag == 0 ) { 
        GPIOA->BSRR = 1 << 5; // PA5 LED On
    }
    else{
        GPIOA->BSRR = 1 << 21; // PA5 LED Off
    }
    /* End Your code */
    
    // IRQ flag clear
    TIM2->SR = ~TIM_SR_UIF;
}

#endif