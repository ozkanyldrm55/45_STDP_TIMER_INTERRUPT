#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

GPIO_InitTypeDef 	  GPIO_InitStruct;
TIM_TimeBaseInitTypeDef   TIM_InitStruct;
NVIC_InitTypeDef     	  NVIC_InitStruct;

int count ;

void GPIO_Config(void) {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOD , &GPIO_InitStruct);
}

void TIM_Config(void) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	/*   SN = ((periyod +1) * (prescaler +1)) / clock speed   */
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1; 	// clock frekansını 1 degerine boluyoruz.
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up; 	// yukari dogru sayacak
	TIM_InitStruct.TIM_Period = 1999;
	TIM_InitStruct.TIM_Prescaler = 41999;
	TIM_InitStruct.TIM_RepetitionCounter = 0; 		// her tasmada tekrarlasin(kac tasmada tekralasin her zaman 1 falasi)

	TIM_TimeBaseInit(TIM3 , &TIM_InitStruct);
	TIM_Cmd(TIM3 , ENABLE);
}

void NVIC_Config(void) { // interrupt yoneticisi
	
	// timer her 1999 degerine kadar saydiginda kesmeye gidecek ve Update interrupt flag (UIF) bayragi kalkacak
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn; 		 	// TIM3 kullanarak kesmeye(interrupt) gidecegini belirtir.
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00; 	// coklu interruptlarda gecerli oncelik icin 
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;		// coklu interruptlarda gecerli oncelik icin

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); // timer ile interrupt i bir arada kullanmak istiyor istiyorsak bu fonk mutlaka baglamaliyiz.
						 // aksi takdirde interrupt a girmayecektir.
	NVIC_Init(&NVIC_InitStruct);
}

void TIM3_IRQHandler() { // her timer tasasi oldugunda yani 1999 oldugunda , Update interrupt flag (UIF) bayragi kalkacak ve fonksiyona gelecek 
	GPIO_ToggleBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

	TIM_ClearITPendingBit(TIM3,TIM_IT_Update); // UIF flag i kalktiginda kendisi tekrardan pasif olmaz.
						   // tekrar interrupt olsun istiyorsak UIF flag ini temizlemeliyiz.	
}

int main(void) {
	GPIO_Config();
	TIM_Config();
	NVIC_Config();
	GPIO_SetBits(GPIOD,GPIO_Pin_12);

  while (1)
  {
	count = TIM_GetCounter(TIM3); // timer degerini izlemek icin olusturduk.(STM Studio dan izleyebiliriz.)
  }
}


void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
