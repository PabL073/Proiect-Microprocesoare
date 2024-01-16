#include "tsi.h"
#include "Uart.h"

#define TSI_CH_MAIN (uint8_t)9
#define TSI_CH_SECONDARY (uint8_t)10

#define SAMPLING_RATE 100

void selectChannel(uint8_t channel);

uint32_t static sum = 0;
uint16_t static count = 0;

void selectChannel(uint8_t channel)
{
	/* Reset TSICH*/
	TSI0->DATA &= ~TSI_DATA_TSICH_MASK;

	/* Select channel*/
	TSI0->DATA |= (TSI_DATA_TSICH_MASK & ((unsigned int)channel << TSI_DATA_TSICH_SHIFT));
}


uint16_t scan(void)
{

	uint16_t cnt = 0;

	selectChannel(TSI_CH_MAIN);

	
	/* Reset out of range bit*/
	TSI0->GENCS &= ~TSI_GENCS_OUTRGF_MASK;

	/* Start scanning */
	TSI0->DATA |= TSI_DATA_SWTS_MASK;

	return cnt;
}

void TSI_init(void)
{
	/* Enable TSI from SIM*/
	SIM->SCGC5 |= SIM_SCGC5_TSI_MASK |	/* Enable TSI*/
				  SIM_SCGC5_PORTB_MASK; /* Enable PortB */

	/*Config TSI*/
	TSI0->GENCS |= TSI_GENCS_TSIEN_MASK | /* Enable TSI*/
				   TSI_GENCS_STPE_MASK |  /* Enable TSI low power functioning ( just in case )*/
				   TSI_GENCS_ESOR_MASK;	  /* End of scan interrupt ( not end of range )*/

	TSI0->GENCS |= TSI_GENCS_TSIIEN_MASK; /* enable tsi interrupts to enable DMA*/

	TSI0->GENCS &= ~TSI_GENCS_STM_MASK; /* Enable software interrupt mode*/

	NVIC_EnableIRQ(TSI0_IRQn);
	NVIC_SetPriority(TSI0_IRQn, 0);

	scan();
}

void TSI_update(void)
{
	/* if no scan is in progress, start one*/
	if ((TSI0->GENCS & TSI_GENCS_SCNIP_MASK) == 0)
		scan();
}

void TSI0_IRQHandler(void)
{

	if (((TSI0->GENCS & TSI_GENCS_EOSF_MASK) >> TSI_GENCS_EOSF_SHIFT) == 1)
	{
		uint16_t i =0;
		TSI_Readings = (TSI0->DATA & TSI_DATA_TSICNT_MASK) >> TSI_DATA_TSICNT_SHIFT;
		TSI0->GENCS |= TSI_GENCS_EOSF_MASK;

		sum += TSI_Readings;

		if (count >= SAMPLING_RATE)
		{

			avg = (uint16_t)(sum / SAMPLING_RATE);

			sum = 0;
			count = 0;
		}

		count++;

		
		for (i=0; i < 2500; i++)
			;

		scan();
	}
}
