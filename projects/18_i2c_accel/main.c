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

int main()
{
	retarget_init();
	i2c_init(0, 1000000U);

	while(1) {
		uint8_t val = 0;
		i2c_read_register(0, 0x0D, &val);
		printf("%x\r\n",val);
	}
	return 1;
}
