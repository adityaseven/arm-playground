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
#include <mma8451q.h>

int main()
{
	struct i2c_handle *h;

	retarget_init();

	h = i2c_get_default(0);
	i2c_init(h, 1000000U);
	i2c_set_slave_addr(h, 0x1D);

	while(1) {
		uint8_t val[2];
		val[0] = WHO_AM_I;
		i2c_write_reg(h, &val[0], 1);
		i2c_read_reg(h, &val[1], 1);
		printf("%x\r\n",val[1]);
	}
	return 1;
}
