#include "Pwm.h"

#define OSCILLOSCOPE_PIN (2) /* PORT A , PIN 1*/
#include "tsi.h"

void TPM2_Init(void)
{

	/* Activarea semnalului de ceas pentru utilizarea LED-ului de culoare rosie*/
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;

	/* Utilizarea alternativei de functionare pentru perifericul TMP  -> TMP2_CH0*/
	PORTB->PCR[OSCILLOSCOPE_PIN] |= PORT_PCR_MUX(3);

	/* Selects the clock source for the TPM counter clock (MCGFLLCLK)  MCGFLLCLK Freq. - 48 MHz - PG. 196*/
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);

	/*Activarea semnalului de ceas pentru modulul TPM*/
	SIM->SCGC6 |= SIM_SCGC6_TPM2(1);

	/* Divizor de frecventa pentru ceasul de intrare*/
	/* PWM CLK -> 48MHz / 128 = 48.000.000 / 128 [Hz] = 375.000 [Hz] = 375 kHz*/
	TPM2->SC |= TPM_SC_PS(7);

	/* LPTPM counter operates in up counting mode. - PG. 553*/
	/* Selects edge aligned PWM*/
	TPM2->SC |= TPM_SC_CPWMS(0);

	/* LPTPM counter increments on every LPTPM counter clock*/
	TPM2->SC |= TPM_SC_CMOD(1);

	TPM2->CONTROLS[0].CnSC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK);
}

void Signal_Control(uint8_t value)
{
	uint32_t extended_min_pulse = (uint32_t)(375 * 0.2);
	uint32_t extended_max_pulse = (uint32_t)(375 * 2.0);
	uint32_t pulse_range = extended_max_pulse - extended_min_pulse;
	static uint8_t duty_cycle = 0;
	static uint8_t increasing = 1; /* 1 for increasing, 0 for decreasing*/
	
	uint8_t aux = (uint8_t)avg;

	/* Reset the counter*/
	TPM2->CNT = 0x0000;

	/* Set the PWM period (20 ms)*/
	TPM2->MOD = 375 * 20;

	
	aux += 0x30;

	/* Interpret value parameter to set the direction or stop*/
	switch (value)
	{
	case 0: /* Slide the servo with TSI Sensor */

		if (aux < 75)
			increasing = 1;
		else
			increasing = 0;
		break;

	case 1:
		increasing = 1;
		break;
	case 2:
		increasing = 0;
		break;
	case 3: /*Toggle direction*/
		increasing = !increasing;
		break;
	}

	/* Set the duty cycle for the servo*/
	TPM2->CONTROLS[0].CnV = extended_min_pulse + (pulse_range * duty_cycle / 40);

	/* Update duty_cycle based on the direction*/
	if (increasing)
	{
		if (duty_cycle < 20)
		{
			duty_cycle++;
		}
	}
	else
	{
		if (duty_cycle > 0)
		{
			duty_cycle--;
		}
	}
}
