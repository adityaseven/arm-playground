/*
 *
 *       Filename:  main.c
 *
 *    Description:  I2C driver with ACCEL testing
 *
 *        Version:  1.0
 *        Created:  08/04/2017 21:56:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Raghava Aditya Renukunta (), adi.raghava3@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */


#include <MKL46Z4.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <limits.h>
#include <i2c.h>
#include <debug_console.h>

#define I2C0_SCL_PORT PORTE
#define I2C0_SCL_PIN  24
#define I2C0_SCL_MUX  5

#define I2C0_SDA_PORT PORTE
#define I2C0_SDA_PIN  25
#define I2C0_SDA_MUX  5

void init_i2c()
{
	SIM->SCGC5 = SIM_SCGC5_PORTC(1) | SIM_SCGC5_PORTD(1) | SIM_SCGC5_PORTE(1);

	I2C0_SCL_PORT->PCR[I2C0_SCL_PIN] = PORT_PCR_MUX(I2C0_SCL_MUX);
	I2C0_SDA_PORT->PCR[I2C0_SDA_PIN] = PORT_PCR_MUX(I2C0_SDA_MUX);

	SIM->SCGC4 |= SIM_SCGC4_I2C0(1);

	i2c_init(I2C0, 1000000U);
}

int main()
{
	retarget_init();
	init_i2c();

	while(1) {
		uint8_t val = 0;
		i2c_read_register(I2C0, 0x0D, &val);
		printf("%x\r\n",val);
	}
	return 1;
}
