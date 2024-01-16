#include "MKL25Z4.h"

void UART0_Transmit(uint8_t data);   /* Functie folostia pentru a trimite un octet catre interfata UART*/
uint8_t UART0_Receive(void);         /* Functie ce returneaza un octet de pe interfata UART, atunci cand acesta exista un buffer*/
void UART0_Init(uint32_t baud_rate); /* Initializare a modulului UART pentru trasmiterea datelor, ce primeste ca parametru baud rate-ul dorit*/
void UART0_IRQHandler(void);

extern uint8_t c;
extern volatile uint8_t START_flag;
extern volatile uint8_t DIRECTION_flag;
