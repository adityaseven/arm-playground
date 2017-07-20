/*
 * =====================================================================================
 *
 *       Filename:  keypad.c
 *
 *    Description:  Added support for keypad
 *
 *        Version:  1.0
 *        Created:  07/19/2017 21:45:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Raghava Aditya Renukunta (), adi.raghava3@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <MKL46Z4.h>
#include <keypad.h>
#include <stdint.h>
#include <stdbool.h>

static struct keypad_attr *key_pad;

static char keypad_keys[ROWS][COLUMNS] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};

static inline void on_line(enum  keypad_output_lines line)
{
	uint8_t pin;
	line = OP_LINE_INDX + line;

	pin = key_pad->line_tbl[line].pin;
	key_pad->line_tbl[line].gpio->PSOR |= (1U << pin);
}

static inline void off_line(enum keypad_output_lines line)
{
	uint8_t pin;
	line = OP_LINE_INDX + line;

	pin = key_pad->line_tbl[line].pin;
	key_pad->line_tbl[line].gpio->PCOR |= (1U << pin);
}

static inline bool is_line_on(enum keypad_input_lines line)
{
	uint8_t pin;
	line = IP_LINE_INDX + line;

	pin = key_pad->line_tbl[line].pin;
	return (key_pad->line_tbl[line].gpio->PDIR & (1U << pin));
}

static inline char get_key(enum keypad_output_lines ko, enum keypad_input_lines ki)
{

	ko = NUM_OP_LINES - ko - 1;
	ki = NUM_IP_LINES - ki - 1;

	return keypad_keys[ko][ki];
}

static void init_kp_port()
{
	int i = 0;
	uint8_t pin;
	uint8_t mux;

	SIM->SCGC5 |= SIM_SCGC5_PORTE(1);

	for (i = 0; i < NUM_OF_LINES; i++) {
		pin = key_pad->line_tbl[i].pin;
		mux = key_pad->line_tbl[i].mux;

		key_pad->line_tbl[i].port->PCR[pin] = PORT_PCR_MUX(mux);
	}

	for (i = IP_LINE_INDX; i < NUM_IP_LINES; i++) {
		pin = key_pad->line_tbl[i].pin;
		//TODO: add pull up config to table?
		key_pad->line_tbl[i].port->PCR[pin] |= (PORT_PCR_PE(1U));
	}
}


static void init_kp_pins()
{
	int i = 0;
	uint8_t pin;

	for (i = IP_LINE_INDX; i < NUM_IP_LINES; i++) {
		pin = key_pad->line_tbl[i].pin;

		key_pad->line_tbl[i].gpio->PDDR &=  ~(1U << pin);
	}

	//Fix this
	for (i = OP_LINE_INDX; i < NUM_OF_LINES; i++) {
		pin = key_pad->line_tbl[i].pin;

		key_pad->line_tbl[i].gpio->PDDR |= (1U << pin);
		key_pad->line_tbl[i].gpio->PCOR |= (1U << pin);
	}
}

static bool scan_key(uint8_t kp_row, uint8_t kp_column, char *c)
{

	bool hit = false;

	on_line(kp_row);
	if (is_line_on(kp_column)) {
		//TODO: Prevent debouncing bit of a hack?
		while(is_line_on(kp_column));
		*c =  get_key(kp_row, kp_column);
		hit = true;
	}
	off_line(kp_row);

	return hit;
}

bool get_pressed_key(char *c)
{
	uint8_t kr, kc;

	for (kr = 0; kr < ROWS; kr++) {
		for (kc = 0; kc < COLUMNS; kc++) {
				if (scan_key(kr, kc, c))
					return true;
			}
	}

	return false;
}

bool init_keypad(struct keypad_attr *kp)
{
	key_pad = kp;
	init_kp_port();
	init_kp_pins();

	return true;
}
