#include "Pit.h"
#include "Uart.h"

#define RED_LED_PIN (18)   /* PORT B*/
#define GREEN_LED_PIN (19) /* PORT B*/
#define BLUE_LED_PIN (1)   /* PORT D*/
static uint8_t state;

void pitInit(void)
{
	/* Activarea semnalului de ceas pentru perifericul PIT*/
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;

	/* Utilizarea semnalului de ceas pentru tabloul de timere*/
	PIT_MCR &= ~PIT_MCR_MDIS_MASK;

	/* Oprirea decrementarii valorilor numaratoarelor in modul debug*/
	PIT->MCR |= PIT_MCR_FRZ_MASK;

	/* Setarea valoarea numaratorului de pe canalul 0 la o perioada de 1 secunda*/
	PIT->CHANNEL[0].LDVAL = 0x911EB7;

	/* Activarea intreruperilor pe canalul 0*/
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
	/* Activarea timerului de pe canalul 0*/
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;

	/* Setarea valoarea numaratorului de pe canalul 1 la o perioada de 10 secunde*/
	PIT->CHANNEL[1].LDVAL = 0x9FFFFF;

	/* Activara intreruperilor pe canalul 1*/
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TIE_MASK;
	/* Activarea timerului de pe canalul 1*/
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK;

	state = 0;

	/* Activarea intreruperii mascabile si setarea prioritatiis*/
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_SetPriority(PIT_IRQn, 5);
	NVIC_EnableIRQ(PIT_IRQn);
}

void PIT_IRQHandler(void)
{

	if (START_flag)
	{

		if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK)
		{
			
			if (DIRECTION_flag == 1)
			{
			
			
			if(state==0)
			{
				GPIOD_PTOR |= (1<<BLUE_LED_PIN);
				state=1;
			}
			else if(state==1)
			{
				GPIOB_PTOR |= (1<<GREEN_LED_PIN);
				GPIOB_PTOR |= (1<<RED_LED_PIN);
				state=2;
			}
			else if(state==2)
			{
				GPIOB_PTOR |= (1<<GREEN_LED_PIN);
				GPIOD_PTOR |= (1<<BLUE_LED_PIN);
				state=3;
			}
			else if(state==3)
			{
				GPIOB_PTOR |= (1<<RED_LED_PIN);
				state=0;
			}
			
		}
		else {
			if(state==0)
			{
				GPIOB_PTOR |= (1<<RED_LED_PIN);
				state=3;
			}
			else if(state==1)
			{
				GPIOD_PTOR |= (1<<BLUE_LED_PIN);
				state=0;
			}
			else if(state==2)
			{
				GPIOB_PTOR |= (1<<GREEN_LED_PIN);
				GPIOB_PTOR |= (1<<RED_LED_PIN);
				state=1;
			}
			else if(state==3)
			{
				GPIOB_PTOR |= (1<<GREEN_LED_PIN);
				GPIOD_PTOR |= (1<<BLUE_LED_PIN);
				state=2;
			}
		}
	
				PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
		}

		if (PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK)
		{
			ADC0->SC1[0] |= ADC_SC1_AIEN_MASK;
			PIT->CHANNEL[1].TFLG &= PIT_TFLG_TIF_MASK;
		}
	}
}

