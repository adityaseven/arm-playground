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

#define KP_IP_PIN1			31U
#define KP_IP_PIN2			19U
#define KP_IP_PIN3			18U
#define KP_IP_PIN4			17U
#define KP_OP_PIN5			16U
#define KP_OP_PIN6			6U
#define KP_OP_PIN7			3U
#define KP_OP_PIN8			2U

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

#define ROWS 4
#define COLUMNS 4

char keypad[ROWS][COLUMNS] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};

static inline void on_line(enum  keypad_output_lines line)
{
	uint8_t pin;
	line = OP_LINE_INDX + line;

	pin = line_tbl[line].pin;
	line_tbl[line].gpio->PSOR |= (1U << pin);
}

static inline void off_line(enum keypad_output_lines line)
{
	uint8_t pin;
	line = OP_LINE_INDX + line;

	pin = line_tbl[line].pin;
	line_tbl[line].gpio->PCOR |= (1U << pin);
}

static inline bool is_line_on(enum keypad_input_lines line)
{
	uint8_t pin;
	line = IP_LINE_INDX + line;

	pin = line_tbl[line].pin;
	return (line_tbl[line].gpio->PDIR & (1U << pin));
}

static inline void hold_line(enum keypad_input_lines line)
{
	uint8_t pin;
	line = IP_LINE_INDX + line ;

	pin = line_tbl[line].pin;
	while(line_tbl[line].gpio->PDIR & (1U << pin));
}

static inline char get_key(enum keypad_output_lines ko, enum keypad_input_lines ki)
{

	ko = NUM_OP_LINES - ko - 1;
	ki = NUM_IP_LINES - ki - 1;

	return keypad[ko][ki];
}

void init_kp_port()
{
	int i = 0;
	uint8_t pin;
	uint8_t mux;

	SIM->SCGC5 |= SIM_SCGC5_PORTE(1);

	for (i = 0; i < NUM_OF_LINES; i++) {
		pin = line_tbl[i].pin;
		mux = line_tbl[i].mux;

		line_tbl[i].port->PCR[pin] = PORT_PCR_MUX(mux);
	}

	for (i = IP_LINE_INDX; i < NUM_IP_LINES; i++) {
		pin = line_tbl[i].pin;
		//TODO: add pull up config to table?
		line_tbl[i].port->PCR[pin] |= (PORT_PCR_PE(1U));
	}
}


void init_kp_pins()
{
	int i = 0;
	uint8_t pin;

	for (i = IP_LINE_INDX; i < NUM_IP_LINES; i++) {
		pin = line_tbl[i].pin;

		line_tbl[i].gpio->PDDR &=  ~(1U << pin);
	}

	//Fix this
	for (i = OP_LINE_INDX; i < NUM_OF_LINES; i++) {
		pin = line_tbl[i].pin;

		line_tbl[i].gpio->PDDR |= (1U << pin);
		line_tbl[i].gpio->PCOR |= (1U << pin);
	}
}


static void scan_key(uint8_t kp_row, uint8_t kp_column) {

	on_line(kp_row);
	if (is_line_on(kp_column)) {
		//TODO: Prevent debouncing bit of a hack?
		hold_line(kp_column);
		printf("%c\r\n", get_key(kp_row, kp_column));
	}
	off_line(kp_row);
}

int main()
{
	//enum keypad_output_lines op_line;
	retarget_init();
	init_leds();
	init_kp_port();
	init_kp_pins();

	while(1) {

		uint8_t kr, kc;

		for (kr = 0; kr < ROWS; kr++) {
			for (kc = 0; kc < COLUMNS; kc++) {
				scan_key(kr,kc);
			}
		}
	}

	return 0;
}

