#include "ClockSettings.h"

uint16_t static base_tick;
uint16_t static tick_50ms;
uint16_t static tick_100ms;
uint16_t static tick_500ms;
uint16_t static tick_1s;

volatile uint8_t flag_50ms;
volatile uint8_t flag_100ms;
volatile uint8_t flag_500ms;
volatile uint8_t flag_1s;
volatile uint8_t flag_5s;

void SystemClock_Configure(void)
{

	/* MCGOUTCLOCK are ca si iesire, selectia FLL*/
	MCG->C1 |= MCG_C1_CLKS(0);

	/* Selectarea ceasului intern ca si referinta pentru sursa FLL*/
	MCG->C1 |= MCG_C1_IREFS_MASK;

	/*Setarea frecventei ceasului digital la 48 Mhz*/
	
	MCG->C4 |= MCG_C4_DRST_DRS(1);
	MCG->C4 |= MCG_C4_DMX32(1);
}

void SystemClockTick_Configure(void)
{

	/* Deoarece imi doresc ca frecventa de generare a semnalului PWM sa tina cont de tick-uri de 1 ms,
	 trebuie realizate urmatoarele configurari
	 48.000.000 pulsuri .......... 1 secunda
		x      pulsuri .......... 10^(-3) secunde
	*/
	SysTick->LOAD = (uint32_t)(48000000UL / 1000 - 1UL);

	NVIC_SetPriority(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);

	/* Resetarea numaratorului digital*/
	SysTick->VAL = 0UL;

	SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | /* Ceasul procesorului utilizat ca referinta (48Mhz)*/
					  SysTick_CTRL_TICKINT_Msk |   /* Atunci cand numaratorul ajunge la 0, va porni handler-ul de intrerupere*/
					  SysTick_CTRL_ENABLE_Msk);	   /* Incarca valoarea pusa in registrul RELOAD si incepe numaratoarea*/
}

void SysTick_Handler(void)
{

	++base_tick;

	if (base_tick >= 50U)
	{

		base_tick = 0U;
		flag_50ms = 1U;

		++tick_50ms;
	}
	if (tick_50ms >= 2U)
	{

		tick_50ms = 0U;
		flag_100ms = 1U;

		++tick_100ms;
	}
	if (tick_100ms >= 5U)
	{

		tick_100ms = 0U;
		flag_500ms = 1U;

		++tick_500ms;
	}
	if (tick_500ms >= 2U)
	{
		tick_500ms = 0U;
		flag_1s = 1U;

		++tick_1s;
	}
	if (tick_1s >= 5U)
	{
		tick_1s = 0U;
		flag_5s = 1U;
	}
}
