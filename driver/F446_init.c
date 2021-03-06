//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クロックの設定
// 2019.2.3 sakai created.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef F446_INIT_C
#define F446_INIT_C

#include "F446_init.h"


// =====================================================================================================================
// HSE外部クロックとPLLの設定 HSE and PLL Setting.
//この関数を実行すると外部クリスタル(16MHz)を使って180MHzで動作します。
// =====================================================================================================================
void HSE_PLL_init() {
	/* HSI ON */
	RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY)) continue;
	
    /* Power supply setup */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;

    /* Enable Over-drive mode */
    PWR->CR |= PWR_CR_ODEN;
    while (!(PWR->CSR & PWR_CSR_ODRDY)) continue;

   /* Switch to Over-drive mode */
    PWR->CR |= PWR_CR_ODSWEN;
    while (!(PWR->CSR & PWR_CSR_ODSWRDY)) continue;

	/*
     * PLL settings 各クロック分配部へは以下のようにクロックが供給されます。
     * AHB  = SYSCLK / 1 = 180MHz
     * APB1 = SYSCLK / 4 = 45MHz
     * APB2 = SYSCLK / 2 = 90MHz
     */
	    RCC->CFGR |= RCC_CFGR_HPRE_DIV1  |
                 RCC_CFGR_PPRE2_DIV2 |
                 RCC_CFGR_PPRE1_DIV4;
    
	/* PLLCLK = HSI(16 MHz) / M * N / P */
    RCC->PLLCFGR = PLL_M | (PLL_N << RCC_PLLCFGR_PLLN_Pos) | (((PLL_P >> 1) -1) << RCC_PLLCFGR_PLLP_Pos) |
                   RCC_PLLCFGR_PLLSRC_HSE | (PLL_Q << RCC_PLLCFGR_PLLQ_Pos);

    /* PLL ON */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait until PLL is locked up */
    while (!(RCC->CR & RCC_CR_PLLRDY)) continue;
    
	/* Increase flash latency */
    FLASH->ACR = FLASH_ACR_LATENCY_5WS;

    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
  }



// =====================================================================================================================
// HSI内蔵クロックとPLLの設定 HSI and PLL Setting.
// この関数を実行すると内部クロックを使って180MHzで動作します。内部クロックのため誤差が大きくなります。
// =====================================================================================================================
void HSI_PLL_init(void) {
	/* HSI ON */
	RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY)) continue;
	
    /* Power supply setup */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;

    /* Enable Over-drive mode */
    PWR->CR |= PWR_CR_ODEN;
    while (!(PWR->CSR & PWR_CSR_ODRDY)) continue;

   /* Switch to Over-drive mode */
    PWR->CR |= PWR_CR_ODSWEN;
    while (!(PWR->CSR & PWR_CSR_ODSWRDY)) continue;

	/*
     * PLL settings
     * AHB  = SYSCLK / 1 = 180MHz
     * APB1 = SYSCLK / 4 = 45MHz
     * APB2 = SYSCLK / 2 = 90MHz
     */
	    RCC->CFGR |= RCC_CFGR_HPRE_DIV1  |
                 RCC_CFGR_PPRE2_DIV2 |
                 RCC_CFGR_PPRE1_DIV4;
    
	/* PLLCLK = HSI(16 MHz) / M * N / P */
    RCC->PLLCFGR = PLL_M | (PLL_N << RCC_PLLCFGR_PLLN_Pos) | (((PLL_P >> 1) -1) << RCC_PLLCFGR_PLLP_Pos) |
                   RCC_PLLCFGR_PLLSRC_HSI | (PLL_Q << RCC_PLLCFGR_PLLQ_Pos);
	
    // TRIM HSI
    RCC->CR |= 0x10 << RCC_CR_HSITRIM_Pos;

    /* PLL ON */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait until PLL is locked up */
    while (!(RCC->CR & RCC_CR_PLLRDY)) continue;
    
	/* Increase flash latency */
    FLASH->ACR = FLASH_ACR_LATENCY_5WS;

	// switch Sysclock to PLL
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
  }

#endif