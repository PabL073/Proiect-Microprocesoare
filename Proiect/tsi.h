#ifndef TSI_MANAGER_H
#define TSI_MANAGER_H

#include "MKL25Z4.h"

extern uint16_t TSI_Readings;
extern uint16_t avg;

void TSI_init(void);
void TSI_update(void);
void TSI0_IRQHandler(void);
uint16_t scan(void);

#endif
