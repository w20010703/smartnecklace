#include "declaration.h"
#include "adc.h"

void adc_init(void)
{
	ADC_InitTypeDef       	ADC_InitStructure;
	ADC_CommonInitTypeDef 	ADC_CommonInitStructure;
	GPIO_InitTypeDef      	GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef		TIM_OCInitStructure;

	/* Enable ADC3 and GPIO clocks ****************************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,  ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	//Timer configuration
	TIM_TimeBaseStructure.TIM_Period		= ADC_CNT-1;
	TIM_TimeBaseStructure.TIM_Prescaler		= ADC_PRESCALE-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode	= TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	// TIM2 channel2 configuration in PWM mode
	TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse       = 1;
	TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_Low;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	//TIM_Cmd(TIM2, ENABLE);
	
	/* Configure ADC1 Channel15 pin as analog input ******************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* ADC Common Init **********************************************************/
	ADC_CommonInitStructure.ADC_Mode 		     = ADC_Mode_Independent;  //ADC_TripleMode_RegSimult;
	ADC_CommonInitStructure.ADC_Prescaler 	     = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode    = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC1 Init ****************************************************************/
	ADC_InitStructure.ADC_Resolution 		   = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode 		   = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode   = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv     = ADC_ExternalTrigConv_T2_CC2; // ADC_ExternalTrigConvEdge_None
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Falling;
	ADC_InitStructure.ADC_DataAlign 		   = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion 	   = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_3Cycles);
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
}

void adc_dma_init(void)
{
  DMA_InitTypeDef				DMA_InitStructure;
  NVIC_InitTypeDef				NVIC_InitStructure;
 
  DMA_StructInit(&DMA_InitStructure);

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,  ENABLE);
 
  DMA_DeInit(DMA2_Stream0);  //Set DMA registers to default values
  DMA_InitStructure.DMA_Channel            = DMA_Channel_0;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR; //Source address
  DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)&ADC1Value[0]; //Destination address
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize         = CAPTURE_BUFFER_SIZE; //Buffer size
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //source size - 16bit
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord; // destination size = 16b
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Enable;
  DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &DMA_InitStructure); //Initialize the DMA
  DMA_Cmd(DMA2_Stream0, ENABLE); //Enable the DMA2 - Stream 0

  NVIC_InitStructure.NVIC_IRQChannel                   = DMA2_Stream0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void adc_dma_start(void)
{
  // Enable the Transfer Complete and Half Transfer Interrupt DMA_IT_HT
  DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
  // Enable ADC1 DMA
  ADC_DMACmd(ADC1, DISABLE);
}

void adc_start_trigger(void)
{
	ADC_DMACmd(ADC1, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	ADC1_completed = false;
}

void DONT_DISCARD DMA2_Stream0_IRQHandler(void)
{
  if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0)) {
		ADC1_completed = true;
		ADC_DMACmd(ADC1, DISABLE);
    DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
  }
}