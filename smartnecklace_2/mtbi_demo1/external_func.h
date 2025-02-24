// adc.c
extern void adc_init(void);
extern void adc_dma_init(void);
extern void adc_dma_start(void);
extern void adc_start_trigger(void);
extern void DONT_DISCARD DMA2_Stream0_IRQHandler(void);
extern void DONT_DISCARD DMA2_Stream2_IRQHandler(void);
extern void DONT_DISCARD DMA2_Stream1_IRQHandler(void);

// fat.c
extern FRESULT fat_mount();
extern FRESULT fat_open_file(FIL *fp, char *filename);
extern void fat_unmount();
extern void fat_close_file(FIL *fp);
extern void fat_printf(FIL *fp, char *str);
extern void fat_gets(FIL *fp, char *str);

// gpio.c
extern void gpio_init();
extern void gpio_read(uint8_t *gpio_val);
extern void gpio_write(uint8_t gpio_val);
extern uint8_t gpio_read_sw1();
extern void gpio_flash_green_led();
extern void gpio_flash_red_led();
extern void gpio_flash_leds();
extern void gpio_red_led_on();
extern void gpio_red_led_off();
extern void gpio_green_led_on();
extern void gpio_green_led_off();
extern void gpio_disco_led_init();
extern void gpio_disco_red_led_on();
extern void gpio_disco_red_led_off();
extern void gpio_disco_green_led_on();
extern void gpio_disco_green_led_off();
extern void gpio_disco_flash_red_led();
extern void gpio_disco_flash_green_led();

// init.c
extern void vars_init();

// uart.c
extern void uart_init();
extern void uart_printf(char *str);
extern void uart_send(uint8_t *data, uint16_t len);

// utils.c
extern void init_delay(void);
extern void init_system_tick();
extern void delay_us(uint32_t t);
extern void delay_ms(uint32_t t);
extern void TM_DELAY_1msHandler(void);