#include "MKL25Z4.h"
volatile extern uint8_t flag;
extern uint16_t analog_input;

void ADC0_Init(void);
int ADC0_Calibrate(void);
uint16_t ADC0_Read(void);
void ADC0_IRQHandler(void);
