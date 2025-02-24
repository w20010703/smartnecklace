#include "declaration.h"
#include "gpio.h"

void gpio_init() 
{
	TM_GPIO_Init(GPIOA, GPIO_Pin_0 | GPIO_Pin_1, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_High);
	TM_GPIO_Init(GPIOB, GPIO_Pin_0 | GPIO_Pin_1, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_High);
}

void gpio_read(uint8_t *gpio_val)
{
	uint16_t val;
	val = TM_GPIO_GetPortInputValue(GPIOB);
	*gpio_val = val & 0x00FF;
}

void gpio_write(uint8_t gpio_val)
{
	TM_GPIO_SetPortValue(GPIOA, gpio_val);
}

uint8_t gpio_read_sw1()
{
	uint8_t dat;
	dat = TM_GPIO_GetInputPinValue(GPIOB, GPIO_Pin_0);
	return(dat);
}

void gpio_flash_green_led()
{
	gpio_green_led_on();
	delay_ms(1000);
	gpio_green_led_off();
	delay_ms(1000);
}

void gpio_flash_red_led()
{
	gpio_red_led_on();
	delay_ms(100);
	gpio_red_led_off();
	delay_ms(100);
}

void gpio_flash_leds()
{
	gpio_red_led_on();
	gpio_green_led_on();
	delay_ms(100);
	gpio_red_led_off();
	gpio_green_led_off();
	delay_ms(100);
	gpio_red_led_on();
	delay_ms(100);
	gpio_red_led_off();
	gpio_green_led_on();
	delay_ms(100);
	gpio_green_led_off();
	gpio_red_led_on();
	delay_ms(100);
	gpio_red_led_off();
	delay_ms(100);
}

void gpio_red_led_on()
{
	TM_GPIO_SetPinValue(GPIOA, GPIO_Pin_1, 1);
}

void gpio_red_led_off()
{
	TM_GPIO_SetPinValue(GPIOA, GPIO_Pin_1, 0);
}

void gpio_green_led_on()
{
	TM_GPIO_SetPinValue(GPIOA, GPIO_Pin_0, 1);
}

void gpio_green_led_off()
{
	TM_GPIO_SetPinValue(GPIOA, GPIO_Pin_0, 0);
}

void gpio_disco_led_init() 
{
	 TM_DISCO_LedInit();
}

void gpio_disco_red_led_on()
{
	TM_DISCO_LedOn(LED_RED);
}

void gpio_disco_red_led_off()
{
	TM_DISCO_LedOff(LED_RED);
}

void gpio_disco_green_led_on()
{
	TM_DISCO_LedOn(LED_GREEN);
}

void gpio_disco_green_led_off()
{
	TM_DISCO_LedOff(LED_GREEN);
}

void gpio_disco_flash_red_led()
{
	for(int i=0; i<5; i++) {
		gpio_disco_red_led_on();
		delay_ms(200);
		gpio_disco_red_led_off();
	}
}

void gpio_disco_flash_green_led()
{
	for(int i=0; i<5; i++) {
		gpio_disco_green_led_on();
		delay_ms(200);
		gpio_disco_green_led_off();
	}
}
            