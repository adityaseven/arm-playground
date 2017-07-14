/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Use Adc to increase frequency of the leds
 *
 *        Version:  1.0
 *        Created:  07/13/2017 21:01:47
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

void adc_init()
{

	//Enable PORTE for the ADC pin
	SIM->SCGC5 |= SIM_SCGC5_PORTE(1);

	//Enable ADC0 itself
	SIM->SCGC6 |= SIM_SCGC6_ADC0(1);

	PORTE->PCR[22] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[22] |= PORT_PCR_MUX(0x0);

	//Configure ADC
	//Clear out ADC0 CFG1
	ADC0->CFG1 &= ~(ADC_CFG1_ADIV_MASK);
	ADC0->CFG1 &= ~(ADC_CFG1_MODE_MASK | ADC_CFG1_ADICLK_MASK);

	//Enable Async Clock
	ADC0->CFG1 |= ADC_CFG1_ADICLK(0x3);

	//Select 16 bit conversion mode
	ADC0->CFG1 |= ADC_CFG1_MODE(0x1);

	//Select divide ratio as 8
	ADC0->CFG1 |= ADC_CFG1_ADIV(0x3);

	//Clear out CFG2
	ADC0->CFG2 &= ~(ADC_CFG2_ADLSTS_MASK | ADC_CFG2_ADHSC_MASK | ADC_CFG2_ADACKEN_MASK);
	ADC0->CFG2 &= ~(ADC_CFG2_MUXSEL_MASK);

	//Enable Async clock
	ADC0->CFG2 |= ADC_CFG2_ADACKEN_MASK;

	//Select ref clock
	ADC0->SC2 &= ~ADC_SC2_REFSEL_MASK;

	//Select Software Trigger
	ADC0->SC2 &= ~ADC_SC2_ADTRG_MASK;

	//Disable continue conversion?
	ADC0->SC3 &= ~ADC_SC3_ADCO_MASK;
}

static inline uint32_t read_light_sensor_adc()
{
	//Trigger
	ADC0->SC1[0] |= ADC_SC1_ADCH(0x3);

	while ((ADC0->SC1[0] & ADC_SC1_COCO_MASK) != ADC_SC1_COCO_MASK);

	return ADC0->R[0];
}

void delay(void)
{
	volatile unsigned int i, j;

	for (i = 0U; i < 50000U; i++) {
		for (j = 0U; j < 100U; j++) {
			__asm__("nop");
		}
	}
}

int main()
{
	retarget_init();
	adc_init();

	while(1) {
		printf("Hello World\r\n");
		printf("adc Value -%x\r\n", read_light_sensor_adc());
		delay();
	}

	return 0;
}

