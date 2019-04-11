#include "F446_init.h"
#include "GPIO_446.h"
//#include "Timer_446.h"
#include "UART_446.h"
#include "printf.h"
//#include "midi_base.h"
#include "fifo.h"
#include "DMA_446.h"
#include "ADC_446.h"


unsigned int message[3] = {0xF9,0xF7,0xF5};
unsigned int out_buf[1] = {0};


// =====================================================================================================================
//  delay func
// =====================================================================================================================
void delay (int a)
{
    volatile int i,j;

    for (i=0 ; i < a ; i++)
    {
        j++;
    }
}

// =====================================================================================================================
//  UART3 IRQ callback Function 
// バッファーが空ならば割り込み継続、バッファーがからでなければ送信
// =====================================================================================================================

void USART2_IRQ_enable(){
    USART2-> CR1 |= USART_CR1_RXNEIE;
}
void USART2_IRQ_disable(){
    USART2-> CR1 &= ~USART_CR1_RXNEIE;    
}
void USART3_IRQHandler() {
    static int flag = 0;
    static unsigned int counter = 0;
    
    printf("%x\n", USART2->DR);
    USART2_IRQ_disable(); // TXEIE Off
    flag =! flag; // toggle flag

    if( flag == 0 ) { 
        GPIOA->BSRR = 1 << 5; // PA5 LED On
    }
    else{
        GPIOA->BSRR = 1 << 21; // PA5 LED Off
    }

}
// =====================================================================================================================
//  UART3 IRQ callback Function 
// バッファーが空ならば割り込み継続、バッファーがからでなければ送信
// =====================================================================================================================
void USART2_IRQHandler() {
    static int flag = 0;
    static unsigned int counter = 0;
    uint8_t data = USART2->DR;
    printf("%x\n", data);
    USART2_IRQ_disable(); // TXEIE Off
    flag =! flag; // toggle flag

    if( flag == 0 ) { 
        GPIOA->BSRR = 1 << 5; // PA5 LED On
    }
    else{
        GPIOA->BSRR = 1 << 21; // PA5 LED Off
    }
}
// =====================================================================================================================
//  ADC
    // PC_1 ADC123_11
    // PB_0 ADC12_8
    // PA_1 ADC123_1
    // PA_0 ADC123_0
    // PC_0 ADC123_10
    // PC_3 ADC123_13
    // PA_7 ADC12_7
    // PC_5 ADC_15
// =====================================================================================================================
void gpio_for_adc() {
    RCC->AHB1ENR |= (0b1 << RCC_AHB1ENR_GPIOAEN_Pos) | (0b1 << RCC_AHB1ENR_GPIOBEN_Pos) | (0b1 << RCC_AHB1ENR_GPIOCEN_Pos);
    GPIOA->MODER |=   (0b11 << GPIO_MODER_MODE0_Pos) | (0b11 << GPIO_MODER_MODE1_Pos) | (0b11 << GPIO_MODER_MODE7_Pos); //| (0b11 << GPIO_MODER_MODE4_Pos) | (0b11 << GPIO_MODER_MODE5_Pos) | (0b11 << GPIO_MODER_MODE6_Pos) | (0b11 << GPIO_MODER_MODE7_Pos); //| (0b11 << GPIO_MODER_MODE2_Pos) | (0b11 << GPIO_MODER_MODE3_Pos) 
    GPIOB->MODER |= (0b11 << GPIO_MODER_MODE0_Pos); //| (0b11 << GPIO_MODER_MODE1_Pos);
    GPIOC->MODER = (0b11 << GPIO_MODER_MODE0_Pos) | (0b11 << GPIO_MODER_MODE1_Pos) | (0b11 << GPIO_MODER_MODE3_Pos) | (0b11 << GPIO_MODER_MODE5_Pos);//| (0b11 << GPIO_MODER_MODE2_Pos) | (0b11 << GPIO_MODER_MODE3_Pos) | (0b11 << GPIO_MODER_MODE4_Pos) | (0b11 << GPIO_MODER_MODE5_Pos) ;
}

// =====================================================================================================================
//  M A I N
// =====================================================================================================================
int main(void)
{
    HSE_PLL_init(); // System Clock:180MHz 
   
    //GPIO_init();
    gpio_for_adc();

    UART2_init(); // 115200HzでNucleo USBから出力 printfで使用可能
    init_printf(PutcUSART2);
    ADC1_init_2();

    while(1){
        delay(1000000);
        printf("1 %x\n",ADC1_read(ADC1_PA7));
        printf("2 %x\n",ADC1_read(ADC1_PB0));
        printf("2 %x\n",ADC1_read(ADC1_PA1));
        printf("4 %x\n",ADC1_read(ADC1_PC0));
        printf("5 %x\n",ADC1_read(ADC1_PC3));
        printf("6 %x\n",ADC1_read(ADC1_PA7));
    };
}