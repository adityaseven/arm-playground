/*
 * =====================================================================================
 *
 *       Filename:  switch.h
 *
 *    Description:  switch setup
 *
 *        Version:  1.0
 *        Created:  07/14/2017 21:55:30
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Raghava Aditya Renukunta (), adi.raghava3@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _SWITCH_H_
#define _SWITCH_H_ value

#define SW1		3U
#define SW3		12U
#define S1PORT  PORTC
#define S3PORT  PORTC
#define S1GPIO  GPIOC
#define S3GPIO  GPIOC

#define SW1_ON (!(S1GPIO->PDIR & (1U << SW1)))
#define SW3_ON (!(S3GPIO->PDIR & (1U << SW3)))

void init_switches();

#endif /* ifndef _SWITCH_H_ */
