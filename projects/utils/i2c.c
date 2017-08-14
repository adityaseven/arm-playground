/*
 * =====================================================================================
 *
 *       Filename:  i2c.c
 *
 *    Description:  i2c driver helper files
 *
 *        Version:  1.0
 *        Created:  08/12/2017 17:39:54
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Raghava Aditya Renukunta (), adi.raghava3@gmail.com
 *   Organization: 
 *
 * =====================================================================================
 */

#include <MKL46Z4.h>
#include <stdint.h>
#include <limits.h>

#include "i2c.h"

static const uint16_t ICR[0x40] = {
	20,   22,   24,   26,   28,
	30,   34,   40,   28,   32,
	36,   40,   44,   48,   56,
	68,   48,   56,   64,   72,
	80,   88,   104,  128,  80,
	96,   112,  128,  144,  160,
	192,  240,  160,  192,  224,
	256,  288,  320,  384,  480,
	320,  384,  448,  512,  576,
	640,  668,  960,  640,  768,
	896,  1024, 1152, 1280, 1536,
	1920, 1280, 1536, 1792, 2048,
	2304, 2560, 3072, 3840
};

void i2c_init(I2C_Type *i2c_b, uint32_t baud)
{
	i2c_b->C1 = 0;

	i2c_set_frequency(i2c_b,  baud);

	i2c_b->C1 = I2C_C1_IICEN(1);
}

void  i2c_set_frequency(I2C_Type *i2c_b, uint32_t baud)
{
	uint32_t clock_rate = DEFAULT_BUS_CLOCK;
	uint32_t c_baud;
	uint8_t m = 0;
	uint8_t i = 0;
	uint8_t b_mul = 0;
	uint8_t b_icr = 0;
	uint32_t error = 0;
	uint32_t error_margin = UINT_MAX;

	for (m = 0; m < 2; ++m) {
		m = 1U << m;

		for (i = 0; i < 0x40; i++) {
			c_baud = clock_rate/ (m * ICR[i]);

			error = (c_baud > baud)? (c_baud - baud): (baud - c_baud);

			if (error < error_margin) {
				b_mul = m;
				b_icr = ICR[i];
				error_margin = error;
				if (error == 0)
					break;
			}
		}

		if (error == 0)
			break;
	}

	i2c_b->F = I2C_F_ICR(b_icr) | I2C_F_MULT(b_mul);
}

bool i2c_read_register(I2C_Type *i2c_b, uint8_t addr, uint8_t *val)
{

	i2c_start(i2c_b);

	i2c_write_byte(i2c_b, 0x1D << 1 |0);
	i2c_wait(i2c_b);
	i2c_is_ack(i2c_b);

	i2c_write_byte(i2c_b, addr);
	i2c_wait(i2c_b);
	i2c_is_ack(i2c_b);

	i2c_do_repeated_start(i2c_b);
	i2c_write_byte(i2c_b, 0x1D << 1|1);
	i2c_wait(i2c_b);
	i2c_is_ack(i2c_b);

	i2c_set_rx_mode(i2c_b);

	i2c_send_nack(i2c_b);
	*val = i2c_read_byte(i2c_b);
	i2c_wait(i2c_b);

	i2c_stop(i2c_b);
	*val = i2c_read_byte(i2c_b);

	//shoud i clear something here?
	//

	return 0;
}

bool i2c_write_register(I2C_Type *i2c_b, uint8_t addr, uint8_t data)
{

	i2c_start(i2c_b);

	//TODO:
	i2c_write_byte(i2c_b, 0x1D << 1|0);
	i2c_wait(i2c_b);

	//TODO Add function to check for errors
	i2c_is_ack(i2c_b);

	i2c_write_byte(i2c_b, addr);
	i2c_wait(i2c_b);
	i2c_is_ack(i2c_b);

	i2c_write_byte(i2c_b, data);
	i2c_wait(i2c_b);
	i2c_is_ack(i2c_b);

	i2c_stop(i2c_b);

	return true;
}

