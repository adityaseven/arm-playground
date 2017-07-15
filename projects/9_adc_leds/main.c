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

#define ADC0_LIGHT_SENSOR_PORT PORTE
#define ADC0_LIGHT_SENSOR_PIN 22
#define ADC0_LIGHT_SENSOR_CHANNEL 0x3

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


//TODO: Maybe do some calibration on this?
void adc_init()
{

	//Enable PORTE for the ADC pin
	SIM->SCGC5 |= SIM_SCGC5_PORTE(1);

	//Enable ADC0 itself
	SIM->SCGC6 |= SIM_SCGC6_ADC0(1);

	PORTE->PCR[ADC0_LIGHT_SENSOR_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[ADC0_LIGHT_SENSOR_PIN] |= PORT_PCR_MUX(0x0);

	//Configure ADC

	//Enable Async Clock Select 16 bit conversion mode and Select divide ratio as 8
	ADC0->CFG1 = (ADC_CFG1_ADICLK(0x3) | ADC_CFG1_MODE(0x1) | ADC_CFG1_ADIV(0x3));

	//Enable Async clock
	ADC0->CFG2 = ADC_CFG2_ADACKEN_MASK;

	ADC0->SC2 = 0;
	ADC0->SC3 = 0;
}

void led_init()
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

void adc_calibrate()
{
	uint16_t cal = 0;

	//Begin Calibration
	ADC0->SC3 |= ADC_SC3_CAL_MASK;

	while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK));

	if (ADC0->SC3 & ADC_SC3_CALF_MASK) {
		printf("Calibration faile \r\n");
		return;
	}

	cal += ADC0->CLP0;
	cal += ADC0->CLP1;
	cal += ADC0->CLP2;
	cal += ADC0->CLP3;
	cal += ADC0->CLP4;
	cal += ADC0->CLPS;

	cal /= 2;
	cal = cal | 0x8000;

	ADC0->PG = cal;

	cal = 0;
	cal += ADC0->CLM0;
	cal += ADC0->CLM1;
	cal += ADC0->CLM2;
	cal += ADC0->CLM3;
	cal += ADC0->CLM4;
	cal += ADC0->CLMS;

	cal /= 2;
	cal |= 0x8000;

	ADC0->MG = cal;
}

static inline uint32_t read_light_sensor_adc()
{
	ADC0->SC1[0] = ADC_SC1_ADCH(ADC0_LIGHT_SENSOR_CHANNEL);

	while ((ADC0->SC1[0] & ADC_SC1_COCO_MASK) != ADC_SC1_COCO_MASK);

	return ADC0->R[0];
}

void delay(uint32_t adc_light)
{
	volatile unsigned int i, j;

	for (i = 0U; i < adc_light; i++) {
		for (j = 0U; j < 1000U; j++) {
			__asm__("nop");
		}
	}
}

int main()
{
	uint32_t adc_light =0;
	retarget_init();
	adc_init();
	adc_calibrate();
	led_init();

	while(1) {
		delay(adc_light);
		GGPIO->PSOR |= (1U << GREEN_LED);
		RGPIO->PSOR |= (1U << RED_LED);

		delay(adc_light);
		adc_light = read_light_sensor_adc();
		GGPIO->PCOR |= (1U << GREEN_LED);
		RGPIO->PCOR |= (1U << RED_LED);
		printf("adc Value: %lu\r\n", adc_light);
	}

	return 0;
}

