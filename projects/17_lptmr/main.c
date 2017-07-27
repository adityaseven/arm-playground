/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Example use of lptmr
 *
 *        Version:  1.0
 *        Created:  07/27/2017 00:54:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Raghava Aditya Renukunta (), adi.raghava3@gmail.com
 *
 * =====================================================================================
 */

#include <MKL46Z4.h>
#include <led.h>

void init_delay()
{
	//Enable LPTMR
	SIM->SCGC5 |= SIM_SCGC5_LPTMR(1);

	//Clear the config
	LPTMR0->CSR = 0;

	//Enable 1KHZ clk and enable bypass
	//TODO: try the 48hz one
	LPTMR0->PSR = LPTMR_PSR_PCS(1) | LPTMR_PSR_PBYP(1);
}

void delay(uint16_t length_ms)
{
	//Set the delay
	LPTMR0->CMR = length_ms;

	//Enable lptmr
	LPTMR0->CSR = LPTMR_CSR_TEN(1);

	//Wait till the compare match flag sets
	while(!(LPTMR0->CSR & LPTMR_CSR_TCF_MASK));

	//Disable the lptmr
	LPTMR0->CSR = 0;
}

int main()
{
	init_delay();
	init_leds();

	while(1) {
		RON;
		GON;
		delay(1000);
		ROFF;
		GOFF;
		delay(1000);
	}
	return 1;
}
