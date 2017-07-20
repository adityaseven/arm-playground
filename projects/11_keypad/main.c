/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  keypad scanning
 *
 *        Version:  1.0
 *        Created:  07/16/2017 22:48:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Raghava Aditya Renukunta (), adi.raghava3@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdbool.h>
#include <MKL46Z4.h>
#include <debug_console.h>
#include <led.h>
#include <keypad.h>

#define KP_IP_PIN1			31U
#define KP_IP_PIN2			19U
#define KP_IP_PIN3			18U
#define KP_IP_PIN4			17U
#define KP_OP_PIN5			16U
#define KP_OP_PIN6			6U
#define KP_OP_PIN7			3U
#define KP_OP_PIN8			2U

static const struct line_info line_tbl[] = {
	{KP_IP_PIN1, PORTE, GPIOE, 1U },
	{KP_IP_PIN2, PORTE, GPIOE, 1U },
	{KP_IP_PIN3, PORTE, GPIOE, 1U },
	{KP_IP_PIN4, PORTE, GPIOE, 1U },
	{KP_OP_PIN5, PORTE, GPIOE, 1U },
	{KP_OP_PIN6, PORTE, GPIOE, 1U },
	{KP_OP_PIN7, PORTE, GPIOE, 1U },
	{KP_OP_PIN8, PORTE, GPIOE, 1U },
};

int main()
{
	struct keypad_attr key_pad;
	char c;
	bool hit = false;

	retarget_init();
	init_leds();

	key_pad.line_tbl = line_tbl;
	init_keypad(&key_pad);

	while(1) {
		hit = get_pressed_key(&c);
		if (hit) {
			printf("%c\r\n", c);
			hit = false;
		}
	}

	return 0;
}

