/*
 * =====================================================================================
 *
 *       Filename:  led.c
 *
 *    Description:  LED init and control file
 *
 *        Version:  1.0
 *        Created:  07/14/2017 21:44:25
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Raghava Aditya Renukunta (), adi.raghava3@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <MKL46Z4.h>
#include <led.h>

void init_leds()
{
	SIM->SCGC5 |= (SIM_SCGC5_PORTD_MASK |
					SIM_SCGC5_PORTE_MASK);

	GPORT->PCR[GREEN_LED] |= PORT_PCR_MUX(1U);
	RPORT->PCR[RED_LED]   |= PORT_PCR_MUX(1U);

	GGPIO->PDDR |= (1U << GREEN_LED);
	RGPIO->PDDR |= (1U << RED_LED);

	GGPIO->PSOR |= (1U << GREEN_LED);
	RGPIO->PSOR |= (1U << RED_LED);
}
