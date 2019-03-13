//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クロックの設定
// 2019.2.3 sakai created.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef F446_INIT_H
#define F446_INIT_H

#include "stm32f446xx.h"

#define PLL_M 8 // HS"E" / PLLM
#define PLL_N 360 // PLLM * PLLN
#define PLL_P 2  // PLLN / PLLP
#define PLL_Q 2 // not use

/*
#define PLL_M 8 // HS"I" / PLLM
#define PLL_N 180 // PLLM * PLLN
#define PLL_P 2  // PLLN / PLLP
#define PLL_Q 2 // not use
*/

// =====================================================================================================================
// HSE外部クロックとPLLの設定 HSE and PLL Setting.
// =====================================================================================================================
void HSE_PLL_init(void);
// =====================================================================================================================
// HSI内蔵クロックとPLLの設定 HSI and PLL Setting.
// =====================================================================================================================
void HSI_PLL_init(void);
#endif