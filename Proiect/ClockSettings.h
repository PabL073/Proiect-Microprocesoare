#include "MKL25Z4.h"

extern volatile uint8_t flag_50ms;
extern volatile uint8_t flag_100ms;
extern volatile uint8_t flag_500ms;
extern volatile uint8_t flag_1s;
extern volatile uint8_t flag_5s;

void SystemClock_Configure(void);
void SystemClockTick_Configure(void);
void SysTick_Handler(void);
