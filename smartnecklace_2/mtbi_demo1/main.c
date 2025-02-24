#include "declaration.h"
#include "main.h"

int main(void) 
{
	bool    print_flag = false;
	char	buf[100];
	double	t1, t2;
	FRESULT ret;
	int     time_value[100];
	int     impact_value[100];
	
    // Initialize system (delay &ticks)
	SystemInit();
	init_delay();
	init_system_tick();
	
	// Initialize variables
	vars_init();
	
	// initial GPIO PB0, and UART
	//TM_GPIO_Init(GPIOB, GPIO_Pin_0, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_High); // to be sync signal
	//TM_USART_Init(USART1, TM_USART_PinsPack_2, 115200); // TX: PB6, RX: PB7
	TM_DISCO_ButtonInit();
	//TM_DISCO_LedInit();
	TM_DISCO_LedOff(LED_BLUE);
	
	// Initialize ADC
	adc_init();
	adc_dma_init();
	adc_dma_start();
	
#if 1
	// Mount drive
	ret = fat_mount();
	if (ret != FR_OK) {
		gpio_disco_flash_red_led();
		if (print_flag) printf("Mount Driver Failed\n");
		exit(-1);
	}	
	gpio_disco_red_led_on(); // Mounted OK, turn on Disco red led
	if(print_flag) printf("Mount Driver OK\n");

	// Open file
	ret = fat_open_file(&fil, "capture_data.txt");
	if (ret != FR_OK) {
		gpio_disco_flash_green_led();
		if (print_flag) printf("Open File Failed\n");
		exit(-1);
	}
	gpio_disco_green_led_on(); // File Open OK, turn on Disco green led
	if(print_flag) printf("Open File OK\n");
#endif

	
	// main loop
while (1) 
	{
		while (TM_GPIO_GetInputPinValue(GPIOB, GPIO_Pin_0) && !TM_DISCO_ButtonPressed())
		{
			TM_DISCO_LedToggle(LED_RED);
			delay_ms(200);
		}
		TM_DISCO_LedOff(LED_RED);
		TM_DISCO_LedOn(LED_BLUE);
		
		// step2: start to capture ADC
		adc_start_trigger();
		while (1) {
			if (ADC1_completed) {	
				TIM_Cmd(TIM2, DISABLE);
				memcpy(ADC1Value_stored, ADC1Value, CAPTURE_BUFFER_SIZE * 2);
				break;
			}			
		}
		TM_DISCO_LedOff(LED_BLUE);
	}
}