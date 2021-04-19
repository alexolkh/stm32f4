#include "stm32f4xx.h"

#define LED_PORT		(GPIOD)

#define LED_GREEN		(12)
#define LED_ORANGE		(13)
#define LED_RED			(14)
#define LED_BLUE		(15)


// HSE->PLL(SW, /M, VCO(xN), /N)->SW(PLL) = sysclock

void RCC_Init(void) {
	/* Active Crystal */
	RCC->CR |= RCC_CR_HSEON;
	while( !(RCC->CR & RCC_CR_HSERDY) );

	/* Switching to HSE */
	RCC->CFGR |= 0x01;
	while( !(RCC->CFGR & RCC_CFGR_SWS_HSE) );
}



void Led_Tgl(uint32_t led) {
	GPIOD->ODR ^= (1 << led);
}


void Delay(uint32_t c) {
	while(c--);
}


int main(void) {

	RCC_Init();
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= (0x01 << (LED_GREEN * 2))  |
					(0x01 << (LED_ORANGE * 2)) |
					(0x01 << (LED_RED * 2)   ) |
					(0x01 << (LED_BLUE * 2)  ) ;


	while(1) {
		Led_Tgl(LED_ORANGE);
		Delay(40000);
		Led_Tgl(LED_GREEN);
		Delay(40000);
		Led_Tgl(LED_RED);
		Delay(40000);
		Led_Tgl(LED_BLUE);
		Delay(40000);
	}

	return 0;
}