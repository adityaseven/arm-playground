/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description: Example pwm program
 *
 *        Version:  1.0
 *        Created:  07/25/2017 12:31:34 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Raghava Aditya Renukunta <adi.raghava3@gmail.com>
 * =====================================================================================
 */

#include <MKL46Z4.h>
#include <led.h>

#define GLED_PWM     TPM0
#define GLED_PWN_CHN 5U
#define GLED_PWN_MUX 4U

#define RLED_PWM     TPM0
#define RLED_PWM_CHN 2U
#define RLED_PWN_MUX 3U

void init_pwm()
{
	uint32_t mod = 0;

	SIM->SCGC6 = SIM_SCGC6_TPM0(1U);
	SIM->SCGC5 = SIM_SCGC5_PORTD(1u) | SIM_SCGC5_PORTE(1U);

	GPORT->PCR[GREEN_LED] |= PORT_PCR_MUX(GLED_PWN_MUX);
	RPORT->PCR[RED_LED] |= PORT_PCR_MUX(RLED_PWN_MUX);

	SIM->SOPT2 = SIM_SOPT1_OSC32KSEL(1U);

	mod = (24000000U/24000U) - 1;

	TPM0->MOD = mod;

	TPM0->CONTROLS[GLED_PWN_CHN].CnSC =
				((1U << TPM_CnSC_ELSA_SHIFT) | (2U << TPM_CnSC_MSA_SHIFT));

	while (TPM0->CONTROLS[GLED_PWN_CHN].CnSC &
			(TPM_CnSC_ELSA_SHIFT | TPM_CnSC_MSA_SHIFT | TPM_CnSC_ELSB_SHIFT | TPM_CnSC_MSB_SHIFT));


	TPM0->CONTROLS[GLED_PWN_CHN].CnV = (mod * 50)/100;
}

int main()
{
	init_pwm();

	while(1);
	return 1;
}
