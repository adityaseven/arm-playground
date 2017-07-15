/*
 * =====================================================================================
 *
 *       Filename:  switch.c
 *
 *    Description:  Switch control
 *
 *        Version:  1.0
 *        Created:  07/14/2017 21:57:54
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Raghava Aditya Renukunta (), adi.raghava3@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <MKL46Z4.h>
#include <switch.h>

void init_switches()
{
	SIM->SCGC5 |= (SIM_SCGC5_PORTC_MASK);

	S1PORT->PCR[SW1] |= PORT_PCR_MUX(1U);
	S3PORT->PCR[SW3] |= PORT_PCR_MUX(1U);

	S1PORT->PCR[SW1] |= (PORT_PCR_PE(1U) | PORT_PCR_PS(1U));
	S3PORT->PCR[SW3] |= (PORT_PCR_PE(1U) | PORT_PCR_PS(1U));

	S1GPIO->PDDR &= ~(1U << SW1);
	S3GPIO->PDDR &= ~(1U << SW3);
}
