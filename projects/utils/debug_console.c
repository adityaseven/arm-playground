/*
 * =====================================================================================
 *
 *       Filename:  debug_console.c
 *
 *    Description:  Setup the debug console here
 *
 *        Version:  1.0
 *        Created:  07/14/2017 21:31:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Raghava Aditya Renukunta (), adi.raghava3@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <MKL46Z4.h>

void retarget_init()
{

	char dummy;

	PORTA->PCR[1] = 0x00000200UL;
	PORTA->PCR[2] = 0x00000200UL;

	/* Enable clock gate UART0 */
	SIM->SCGC4 |=   1UL << 10;

	/* Choose MCGFLLCLK/2 (48MHz/2) */
	SIM->SOPT2 |= 1UL << 16;
	SIM->SOPT2 &= ~(1UL << 27);
	SIM->SOPT2 |=   1UL << 26;

	/* Baudrate = clock/(SBR[12:0] x (OSR + 1))   */
	/* OSR = 15, clock = 24MHz, baudrate = 115200 *///ask: can't config another baund rate.
	/* -> SBR = 13 */
	/* LBKDIE = 0, RXEDGIE = 0, SBNS = 0 */
	UART0->BDH = 0x00;
	UART0->BDL = 0x1A;//26 ???

	/* UART0 control */
	UART0->C1 = 0x00;
	UART0->C2 = 0x00;
	UART0->C3 = 0x00;
	UART0->C4 = 0x0F; /* OSR = 15 */
	UART0->C5 = 0x00;

	/* Enable UART0 transmit/receive */
	UART0->C2 |= 0x03 << 2;

	/* Dummy read to ensure UART0
	   receive buffer is empty already */
	dummy = UART0->D;
	(void)dummy;
}

int _write(int fd, char *ptr, int len)
{
	while(--len) {
		while(!(UART0->S1 & UART_S1_TDRE_MASK ));
		UART0->D = *(ptr++);
	};

	return len;
}
