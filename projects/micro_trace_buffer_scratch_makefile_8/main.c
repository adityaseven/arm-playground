/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Example program using micro trace buffer
 *
 *        Version:  1.0
 *        Created:  07/11/2017 07:27:11
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Raghava Aditya Renukunta (), adi.raghava3@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#include <MKL46Z4.h>

#define RED_LED   29U
#define GREEN_LED 5U
#define RPORT  PORTE
#define GPORT  PORTD
#define RGPIO  GPIOE
#define GGPIO  GPIOD

#define SW1		3U
#define SW3		12U
#define S1PORT  PORTC
#define S3PORT  PORTC
#define S1GPIO  GPIOC
#define S3GPIO  GPIOC

#define MTB_SIZE 0x1000

#define MTB_ALIGNMENT 64
unsigned char __attribute__((section (".mtb_buf"))) mtb_buf[MTB_SIZE] __attribute__ ((aligned (MTB_ALIGNMENT)));

void setup_mtb()
{
	MTB->MASTER = MTB_MASTER_EN(1) | MTB_MASTER_MASK(7);
}

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

int main()
{

	setup_mtb();
	init_leds();
	init_switches();


	return 0;
}

