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
	gpio_disco_led_init();
	if(print_flag) printf("Disco LEDs OK\n");

	// Initialize ADC
	adc_init();
	adc_dma_init();
	adc_dma_start();
	if(print_flag) printf("ADC OK\n");

	// Mount drive
	ret = fat_mount();
	if(ret != FR_OK) {
		gpio_disco_flash_red_led();
		if(print_flag) printf("Mount Driver Failed\n");
		exit(-1);
	}	
	gpio_disco_red_led_on(); // Mounted OK, turn on Disco red led
	if(print_flag) printf("Mount Driver OK\n");

	// Open file
	ret = fat_open_file(&fil, "capture_data.txt");
	if(ret != FR_OK) {
		gpio_disco_flash_green_led();
		if(print_flag) printf("Open File Failed\n");
		exit(-1);
	}
	gpio_disco_green_led_on(); // File Open OK, turn on Disco green led
	if(print_flag) printf("Open File OK\n");

	// Wait for sw1 on
#if 0
	while(1) {	
		//gpio_flash_green_led();
		if(gpio_read_sw1() == 0) {
			break;
		}
	}
#endif
	while (TM_GPIO_GetInputPinValue(GPIOB, GPIO_Pin_0) && !TM_DISCO_ButtonPressed())
	{
		TM_DISCO_LedToggle(LED_RED);
		delay_ms(200);
	}
	
	gpio_disco_red_led_off();
	
	
	//gpio_green_led_off();

	if(print_flag) printf("Start Trigger ADC\n");
	adc_start_trigger();
	while(1) {
		if(ADC1_completed && ADC2_completed && ADC3_completed && !adc_capture_completed_flag) {	
			TIM_Cmd(TIM2, DISABLE);
			memcpy(ADC1Value_stored, ADC1Value, CAPTURE_BUFFER_SIZE*2);
			memcpy(ADC2Value_stored, ADC2Value, CAPTURE_BUFFER_SIZE*2);
			memcpy(ADC3Value_stored, ADC3Value, CAPTURE_BUFFER_SIZE*2);
			adc_capture_completed_flag = true;
			adc_start_trigger();
		}
		if(adc_capture_completed_flag) {
			t1 = SysTimer;
			for(int i=0; i<CAPTURE_BUFFER_SIZE; i++) {
				sprintf(buf, "[%06d]: %4d %4d %4d\n", capture_index++, ADC1Value_stored[i], ADC2Value_stored[i], ADC3Value_stored[i]);
				fat_printf(&fil, buf);
			}
			t2 = SysTimer;
			if(print_flag) printf("write file time=%f\n", t2-t1);
			if(capture_index >= CAPTURE_POINTS) break;
			adc_capture_completed_flag = false;			
		}
	}
	TM_DISCO_LedOff(LED_GREEN);
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