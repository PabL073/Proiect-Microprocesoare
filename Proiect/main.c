#include "Gpio.h"
#include "Pit.h"
#include "Adc.h"
#include "Uart.h"
#include "Pwm.h"
#include "ClockSettings.h"
#include "tsi.h"

uint16_t avg = 0;
uint16_t TSI_Readings = 0;
uint8_t c = 'y';

int main(void)
{
	uint8_t parte_fractionara1 = 0;
	uint8_t parte_fractionara2 = 0;
	uint8_t parte_zecimala = 0;
	START_flag = 1;

	SystemClockTick_Configure();

	TSI_init();
	UART0_Init(9600);
	ADC0_Init();
	ledInit();
	pitInit();
	TPM2_Init();


	while (1)
	{
		if (flag_50ms)
		{
			Signal_Control(parte_zecimala);
			flag_50ms = 0U;
		}

		
		if (START_flag)
		{
				if (flag)
			{
				
				float measured_voltage = (analog_input * 3.3f) / 4095;

				parte_zecimala = (uint8_t)measured_voltage;

				parte_fractionara1 = ((uint8_t)(measured_voltage * 10)) % 10;
				parte_fractionara2 = ((uint8_t)(measured_voltage * 100)) % 10;
				UART0_Transmit(parte_zecimala + 0x30);
				UART0_Transmit('.');
				UART0_Transmit(parte_fractionara1 + 0x30);
				UART0_Transmit(parte_fractionara2 + 0x30);
				UART0_Transmit('V');
				UART0_Transmit(0x0A);
				UART0_Transmit(0x0D);

				flag = 0;
			}
		}
	}
}
