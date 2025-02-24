#include "declaration.h"
#include "utils.h"

void init_delay(void)
{
	TM_DELAY_Init();
}

void init_system_tick()
{
	SysTimer = 0.0;
	SysTick_Config(SystemCoreClock / 1000);
}

void delay_us(uint32_t t)
{
	Delay(t);
}

void delay_ms(uint32_t t)
{
	Delayms(t);
}

void TM_DELAY_1msHandler(void)
{
	SysTimer = SysTimer + 0.001;
}