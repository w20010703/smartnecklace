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
    
	if(print_flag) printf("TBI Warning System\n");
	
	TM_DISCO_ButtonInit();

	// Initialize variables
	vars_init();


	// Initialize GPIO (sw & leds)
	gpio_init();    	
	/*
	for(int i=0; i<10; i++) {
		gpio_flash_leds();
	}
	gpio_red_led_off();
	gpio_green_led_off(); 
	delay_ms(1000);
	if(print_flag) printf("LEDs OK\n");
	*/

	
	// Initialize DISCO LEDs
	TM_DISCO_LedInit();
	TM_DISCO_LedOff(LED_BLUE);
	TM_DISCO_LedOff(LED_RED);
	TM_DISCO_LedOff(LED_GREEN);
	if(print_flag) printf("Disco LEDs OK\n");

	// Initialize ADC
	adc_init();
	adc_dma_init();
	adc_dma_start();
	if(print_flag) printf("ADC OK\n");

#if 1
	// Mount drive
	ret = fat_mount();
	if(ret != FR_OK) {
		if(print_flag) printf("Mount Driver Failed\n");
		exit(-1);
	}	
	TM_DISCO_LedOn(LED_RED); // Mounted OK, turn on Disco red led
	if(print_flag) printf("Mount Driver OK\n");

	// Open file
	ret = fat_open_file(&fil, "capture_data.txt");
	if(ret != FR_OK) {
		if(print_flag) printf("Open File Failed\n");
		exit(-1);
	}
#endif

	TM_DISCO_LedOn(LED_GREEN); // File Open OK, turn on Disco green led
	if(print_flag) printf("Open File OK\n");

	// Wait for sw1 on
	while(1) {	
		//gpio_flash_green_led();
		while(!TM_DISCO_ButtonPressed())
		{
			TM_DISCO_LedOn(LED_BLUE);
		}
		
		TM_DISCO_LedOff(LED_RED);
		TM_DISCO_LedOff(LED_GREEN);
		TM_DISCO_LedOff(LED_BLUE);
		
		adc_start_trigger();
		while (1) {
			if (ADC1_completed) {	
				TIM_Cmd(TIM2, DISABLE);
				memcpy(ADC1Value_stored, ADC1Value, CAPTURE_BUFFER_SIZE * 2);
				break;
			}	
		
		/*if(gpio_read_sw1() == 0) {
			break;
		}*/
	}
		
	//gpio_disco_red_led_off();
	
	//TM_GPIO_SetPinValue(GPIOB, GPIO_Pin_6, 0);
	//TM_GPIO_SetPinValue(GPIOB, GPIO_Pin_7, 0);
	
	//gpio_green_led_off();

	//if(print_flag) printf("Start Trigger ADC\n");
	//adc_start_trigger();
	/*while(1) {
		if(ADC1_completed && !adc_capture_completed_flag) {	
			TIM_Cmd(TIM2, DISABLE);
			memcpy(ADC1Value_stored, ADC1Value, CAPTURE_BUFFER_SIZE*2);
			//memcpy(ADC2Value_stored, ADC2Value, CAPTURE_BUFFER_SIZE*2);
			//memcpy(ADC3Value_stored, ADC3Value, CAPTURE_BUFFER_SIZE*2);
			adc_capture_completed_flag = true;
			adc_start_trigger();
		}*/
		if(adc_capture_completed_flag) {
			t1 = SysTimer;
			for(int i=0; i<CAPTURE_BUFFER_SIZE; i++) {
				sprintf(buf, "[%06d]: %4d\n", capture_index++, ADC1Value_stored[i]);
				fat_printf(&fil, buf);
			}
			t2 = SysTimer;
			if(print_flag) printf("write file time=%f\n", t2-t1);
			if(capture_index >= CAPTURE_POINTS) break;
			adc_capture_completed_flag = false;			
		}
	}

	
	TM_DISCO_LedOff(LED_BLUE);
	if(print_flag) printf("Capture Completed\n");

	// Close file
	fat_close_file(&fil);
	if(print_flag) printf("File Close OK\n");

	// Un-Mount drive
	fat_unmount();
	if(print_flag) printf("Un-Mount Disk OK\n");

    while (1) {
 
    }
}