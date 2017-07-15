/*
 * =====================================================================================
 *
 *       Filename:  led.h
 *
 *    Description:  LED init and control header file
 *
 *        Version:  1.0
 *        Created:  07/14/2017 21:44:42
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Raghava Aditya Renukunta (), adi.raghava3@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _LED_H_
#define _LED_H_

#define RED_LED   29U
#define GREEN_LED 5U
#define RPORT  PORTE
#define GPORT  PORTD
#define RGPIO  GPIOE
#define GGPIO  GPIOD

#define RON  (RGPIO->PCOR |= (1U << RED_LED))
#define GON  (GGPIO->PCOR |= (1U << GREEN_LED))
#define ROFF (RGPIO->PSOR |= (1U << RED_LED))
#define GOFF (GGPIO->PSOR |= (1U << GREEN_LED))

void init_leds();

#endif /* ifndef _LED_H_ */

