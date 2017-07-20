/*
 * =====================================================================================
 *
 *       Filename:  keypad.h
 *
 *    Description:  Support for keypad, (4 x 4)
 *
 *        Version:  1.0
 *        Created:  07/19/2017 21:43:53
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Raghava Aditya Renukunta (), adi.raghava3@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */
#ifndef _KEYPAD_H_
#define _KEYPAD_H_

#include <stdbool.h>

#define NUM_IP_LINES 4
#define IP_LINE_INDX 0

#define NUM_OP_LINES 4
#define OP_LINE_INDX  4

#define NUM_OF_LINES (NUM_IP_LINES + NUM_OP_LINES)

struct line_info {
	uint8_t pin;
	PORT_Type *port;
	GPIO_Type *gpio;
	uint8_t mux;
};

enum keypad_input_lines {
	line1 = 0,
	line2 = 1,
	line3 = 2,
	line4 = 3
};

enum keypad_output_lines {
	line5 = 0,
	line6 = 1,
	line7 = 2,
	line8 = 3
};

#define ROWS 4
#define COLUMNS 4

struct keypad_attr {
	const struct line_info *line_tbl;
};

bool init_keypad(struct keypad_attr *kp);
bool get_pressed_key(char *c);

#endif /* ifndef _KEYPAD_H_ */


