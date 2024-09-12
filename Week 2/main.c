// Add Header file
#include "Board_LED.h"										//Board Support LED
#include "stm32f4xx.h" 										//Device Header

//System core clock setup
void SystemCoreClockConfigure(void) {
	RCC->CR |= ((uint32_t)RCC_CR_HSION);		//Enable HSI
	while((RCC->CR & RCC_CR_HSIRDY)==0);		//Wait for HSI Ready
	
	RCC->CFGR = RCC_CFGR_SW_HSI;						//HSI is system clock
	//Wait for HSI used as system clock
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);
	
	FLASH->ACR  = FLASH_ACR_PRFTEN;					//Enable Prefatch Buffer
	FLASH->ACR |= FLASH_ACR_ICEN; 					//Introduction cache enable
	FLASH->ACR |= FLASH_ACR_DCEN;						//Data cache enable
	FLASH->ACR |= FLASH_ACR_LATENCY_5WS;		//Flash 5 wait state
	
	RCC->CFGR	 |= RCC_CFGR_HPRE_DIV1;				//HCLK = SYSCLK
	RCC->CFGR	 |= RCC_CFGR_PPRE1_DIV4;			//APB1 = HCLK/4
	RCC->CFGR	 |= RCC_CFGR_PPRE2_DIV2;			//APB2 = HCLK/2
	RCC->CR		 &=	~RCC_CR_PLLON;						//Disable PLL
	
	//PLL configuration: VCO = HSI/M * N, Sysclk = VCO/P
	RCC->PLLCFGR = ( 16ul 										|			//PLL_M = 16
									(384ul 	<< 	6	)						|			//PLL_N = 384
									(  3ul	<<	16)						|			//PLL_P = 8
									(RCC_PLLCFGR_PLLSRC_HSI)	|			//PLL_SRC = HSI
									(  8ul	<<	24)						);  	//PLL_Q = 8
	
	RCC->CR 	 |=	RCC_CR_PLLON;											//Enable PLL
	while((RCC->CR	&	RCC_CR_PLLRDY) == 0) __NOP();	//Wait till PLL is ready
	
	//Select PLL as system clock source
	RCC->CFGR  &=	~RCC_CFGR_SW;
	RCC->CFGR	 |=	RCC_CFGR_SW_PLL;
	
	//Wait till PLL is system clock src
	while((RCC->CFGR & RCC_CFGR_SWS)	!= RCC_CFGR_SWS_PLL);
}

volatile uint32_t msTicks;

//SysTick_Handler
void SysTick_Handler(void){
	msTicks++;
}

//Delay: delays a number of Systicks
void Delay (uint32_t dlyTicks){
	uint32_t	curTicks;
	
	curTicks = msTicks;
	while((msTicks - curTicks) < dlyTicks)	{__NOP();}
}

int main (void){
	int32_t max_num = LED_GetCount();				//LED_GetCount: number of LEDs on Board
	int32_t num = 0;
	
	SystemCoreClockConfigure();							//Configure HSI as System Clock
	SystemCoreClockUpdate();								//Change System Core Clock
	
	LED_Initialize();												//Initialize I/O port for LED
	SysTick_Config(SystemCoreClock / 1000);	//SysTick 1 msec intterrupts
	
	for(;;){
		LED_On(num);													//Turn specified LED on
		Delay(500);														//Wait 500ms
		LED_Off(num);													//Turn specified LED off
		Delay(500);														//Wait 500ms
		
		num++;																//Change LED number
		if (num >= max_num){									
			num = 0;														//Restart with first LED
		}
	}
}