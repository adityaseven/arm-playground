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
#include <led.h>
#include <switch.h>

#define MTB_SIZE 0x1000

#define MTB_ALIGNMENT 64
//unsigned char __attribute__((section (".mtb_buf"))) mtb_buf[MTB_SIZE] __attribute__ ((aligned (MTB_ALIGNMENT)));

void setup_mtb()
{
	MTB->MASTER = MTB_MASTER_EN(1) | MTB_MASTER_MASK(8);
}

void delay(uint32_t c)
{
	volatile  uint32_t i,j;

	for (i = 0U; i < 1000U; i++) {
		for (j = 0U; j < c; j++) {
			__asm__("nop");
		}
	}
}

void sequence0()
{
	RON;
	GON;
	delay(4000);
	ROFF;
	GOFF;
	delay(4000);
}

void sequence1()
{
	RON;
	GON;
	delay(2000);
	ROFF;
	GOFF;
	delay(2000);
}

void sequence2()
{
	RON;
	delay(200);
	GON;
	delay(200);
	ROFF;
	delay(200);
	GOFF;
	delay(200);
}

int main()
{
	uint8_t sw_count = 0;
	setup_mtb();
	init_leds();
	init_switches();

	while (1) {
		sw_count++;

		switch(sw_count) {
		case 1:
			sequence0();
			break;
		case 2:
			sequence1();
			break;
		case 3:
			sequence2();
			break;
		default:
			sw_count = 0;
		}
	}


	return 0;
}

