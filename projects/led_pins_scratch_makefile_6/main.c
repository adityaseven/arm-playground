/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  buttons and leds
 *
 *        Version:  1.0
 *        Created:  07/09/2017 14:45:10
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdint.h>
#include "MKL46Z4.h"

#define RED_LED 29U
#define GREEN_LED 5U

#define SW1 3U
#define SW3 12U

void delay(void)
{
	volatile  uint32_t i,j;

	for (i = 0U; i < 50000U; i++) {
		for (j = 0U; j < 10U; j++) {
			__asm__("nop");
		}
	}
}

int main(void)
{
	//Enable port clocks
	SIM->SCGC5 |= (SIM_SCGC5_PORTC_MASK  |
					SIM_SCGC5_PORTE_MASK |
					SIM_SCGC5_PORTD_MASK);

	//Enable mux
	PORTD->PCR[GREEN_LED]  = PORT_PCR_MUX(1U);
	PORTE->PCR[RED_LED] = PORT_PCR_MUX(1U);

	PORTC->PCR[SW1]  = PORT_PCR_MUX(1U);
	PORTC->PCR[SW3]  = PORT_PCR_MUX(1U);
	PORTC->PCR[SW1] |= (PORT_PCR_PE(1U) | PORT_PCR_PS(1U));
	PORTC->PCR[SW3] |= (PORT_PCR_PE(1U) | PORT_PCR_PS(1U));

	//set direction as output
	GPIOE->PDDR = (1U << RED_LED);
	GPIOD->PDDR = (1U << GREEN_LED);

	GPIOE->PCOR = (1U << RED_LED);
	GPIOD->PCOR = (1U << GREEN_LED);

	GPIOC->PDDR = 0U;

	while(1) {
		if (GPIOC->PDIR & (1U << SW1))
			GPIOD->PSOR = (1U << GREEN_LED);
		 else
			GPIOD->PCOR = (1U << GREEN_LED);

		if (GPIOC->PDIR & (1U << SW3))
			GPIOE->PSOR = (1U << RED_LED);
		else
			GPIOE->PCOR = (1U << RED_LED);
	}
	return 0;
}


