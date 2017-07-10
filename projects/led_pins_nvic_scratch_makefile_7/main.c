/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description: Led switch with interrupts
 *
 *        Version:  1.0
 *        Created:  07/09/2017 21:06:30
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Raghava Aditya Renukunta (), adi.raghava3@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */


#include "MKL46Z4.h"

#define RED_LED 29U
#define GREEN_LED 5U

#define SW1 3U
#define SW3 12U

void PORTC_PORTD_IRQHandler(void)
{
	static uint8_t rled_on = 0;
	static uint8_t gled_on = 0;
	//Check if SW1 interrupt trigger
	if ((PORTC->PCR[SW1] >> PORT_PCR_ISF_SHIFT) & 0x1U) {
		if(rled_on) {
			GPIOE->PSOR = (1U << RED_LED);
			rled_on = 0;
		} else {
			GPIOE->PCOR = (1U << RED_LED);
			rled_on = 1;
		}
		//Clear the interrrupt
		PORTC->PCR[SW1] |= PORT_PCR_ISF(1);
	}

	if ((PORTC->PCR[SW3] >> PORT_PCR_ISF_SHIFT) & 0x1U) {
		if(gled_on) {
			GPIOD->PSOR = (1U << GREEN_LED);
			gled_on = 0;
		} else {
			GPIOD->PCOR = (1U << GREEN_LED);
			gled_on = 1;
		}
		PORTC->PCR[SW3] |= PORT_PCR_ISF(1);
	}
}

int main()
{
	//Enable port clocks
	SIM->SCGC5 |= (SIM_SCGC5_PORTC_MASK  |
					SIM_SCGC5_PORTE_MASK |
					SIM_SCGC5_PORTD_MASK);

	//Enable mux
	PORTD->PCR[GREEN_LED]  = PORT_PCR_MUX(1U);
	PORTE->PCR[RED_LED] = PORT_PCR_MUX(1U);

	//set direction as output
	GPIOE->PDDR = (1U << RED_LED);
	GPIOD->PDDR = (1U << GREEN_LED);

	//clear the leds
	GPIOE->PSOR = (1U << RED_LED);
	GPIOD->PSOR = (1U << GREEN_LED);

	//enable sw1 gpio pins
	PORTC->PCR[SW1]  = PORT_PCR_MUX(1U);
	PORTC->PCR[SW3]  = PORT_PCR_MUX(1U);

	//Enable nvic switch interrupt
	NVIC_EnableIRQ(PORTC_PORTD_IRQn);

	//Enable internal pull up registers for the switches
	PORTC->PCR[SW1] |= (PORT_PCR_PE(1U) | PORT_PCR_PS(1U));
	PORTC->PCR[SW3] |= (PORT_PCR_PE(1U) | PORT_PCR_PS(1U));

	//Enable interrupts
	PORTC->PCR[SW1] |= PORT_PCR_IRQC(0xA);
	PORTC->PCR[SW3] |= PORT_PCR_IRQC(0xA);

	//set the switch directions as input, this is where they are activated
	GPIOC->PDDR &= ~(1U << SW1);
	GPIOC->PDDR &= ~(1U << SW3);

	while(1);
	return 0;
}

