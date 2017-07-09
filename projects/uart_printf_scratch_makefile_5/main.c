/*
 * Copyright (c) 12.2013, Martin Kojtal (0xc0170)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "main.h"
#include "MKL46Z4.h"
#include <stdio.h>

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

int main(void)
{

	retarget_init();

    while (1) {
        printf("Hello World\r\n");
        delay();
    }
}

void delay(void)
{
    volatile unsigned int i,j;

    for (i = 0U; i < 50000U; i++) {
        for (j = 0U; j < 100U; j++) {
            __asm__("nop");
        }
    }
}

