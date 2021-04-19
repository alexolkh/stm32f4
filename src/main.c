#include "stm32f4xx.h"

#define LED_PORT		(GPIOD)

#define LED_GREEN		(12)
#define LED_ORANGE		(13)
#define LED_RED			(14)
#define LED_BLUE		(15)


// HSE->PLL(SW, /M, VCO(xN), /N)->SW(PLL) = sysclock

void RCC_Init(void) {
	/* Active Cristal */
	RCC->CR |= RCC_CR_HSEON;
	while( !(RCC->CR & RCC_CR_HSERDY) );

	/* Switching to HSE */
	RCC->CFGR |= 0x01;
	while( !(RCC->CFGR & RCC_CFGR_SWS_HSE) );
}


void I2S_Init() {
	uint32_t pllm = 20;
	uint32_t pllI2SN = 60;
	uint32_t pllI2SR = 2;

	/* Конфигурациия источника тактирования */
	RCC->PLLCFGR &= ~((1 << 6) -1);
	RCC->PLLCFGR |= pllm | RCC_PLLCFGR_PLLSRC_HSE;

	RCC->PLLI2SCFGR  = (pllI2SN << 6) | (pllI2SR << 28);

	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;
	
	RCC->CR |= RCC_CR_PLLI2SON;
	while( !(RCC->CR & RCC_CR_PLLI2SRDY) );

	/* Инициализация выводов */
	#define I2S_GPIO_SCK_SD		(GPIOC)
	#define I2S_GPIO_WS			(GPIOA)
	#define I2S_SCK				(10)
	#define I2S_SD				(12)
	#define I2S_WS				(4)
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;
	RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;

	I2S_GPIO_SCK_SD->MODER |= (0x02 << I2S_SCK) | (0x02 << I2S_SD);
	I2S_GPIO_WS->MODER 	   |= (0x02 << I2S_WS);

	I2S_GPIO_SCK_SD->OSPEEDR |= (0x02 << I2S_SCK) | (0x02 << I2S_SD);
	I2S_GPIO_WS->OSPEEDR 	 |= (0x02 << I2S_WS);

	I2S_GPIO_SCK_SD->AFR[1] |= (0x06 << (I2S_SCK * 4 - 32)) | (0x06 << (I2S_SD * 4 - 32));
	I2S_GPIO_WS->AFR[0] 	|= (0x02 << (I2S_WS * 4));

}


void GpioD_Init(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

	GPIOD->MODER |= (0x01 << (LED_GREEN * 2))  |
					(0x01 << (LED_ORANGE * 2)) |
					(0x01 << (LED_RED * 2)   ) |
					(0x01 << (LED_BLUE * 2)  ) ;
}

void Led_Tgl(uint32_t led) {
	GPIOD->ODR ^= (1 << led);
}


void Delay(uint32_t c) {
	while(c--);
}


int main(void) {

	RCC_Init();
	GpioD_Init();

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