/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  pit example
 *
 *        Version:  1.0
 *        Created:  07/23/2017 11:04:39
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
#include <debug_console.h>
#include <stdbool.h>

#define usec_to_count(us) (us * (DEFAULT_BUS_CLOCK/1000000U))

void PIT_IRQHandler()
{
	RTOG;
	GTOG;

	PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;
}

void init_pit(uint32_t us)
{

	SIM->SCGC6 = SIM_SCGC6_PIT_MASK;

	PIT->MCR &= ~(PIT_MCR_MDIS_MASK);
	PIT->MCR |= (PIT_MCR_FRZ_MASK);

	PIT->CHANNEL[0].LDVAL = usec_to_count(us);
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;

	NVIC_EnableIRQ(PIT_IRQn);

	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
}

int main()
{
	init_leds();
	init_pit(1000000);

	while(1);

	return 1;
}
