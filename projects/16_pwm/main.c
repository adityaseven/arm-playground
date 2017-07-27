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
#define GLED_PWM_CHN 5U
#define GLED_PWM_MUX 4U

#define RLED_PWM     TPM0
#define RLED_PWM_CHN 2U
#define RLED_PWM_MUX 3U

void init_pwm()
{
	uint32_t mod = 0;

	SIM->SCGC5 = SIM_SCGC5_PORTD(1u) | SIM_SCGC5_PORTE(1U);

	GPORT->PCR[GREEN_LED] |= PORT_PCR_MUX(GLED_PWM_MUX);
	RPORT->PCR[RED_LED] |= PORT_PCR_MUX(RLED_PWM_MUX);

	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1u) | SIM_SOPT2_PLLFLLSEL(1));

	SIM->SCGC6 = SIM_SCGC6_TPM0(1U);

	mod = (DEFAULT_SYSTEM_CLOCK/24000U) - 1;

	TPM0->SC = 0;
	TPM0->CONF = 0;

	TPM0->SC &= ~TPM_SC_CPWMS_MASK;

	TPM0->MOD = mod;

	TPM0->SC = ((TPM0->SC & ~(TPM_SC_CMOD_MASK)) | TPM_SC_CMOD(1U));
}

void init_pwm_chn(uint8_t chn)
{
	TPM0->CONTROLS[chn].CnSC =
				((1U << TPM_CnSC_ELSA_SHIFT) | (2U << TPM_CnSC_MSA_SHIFT));

	while (!(TPM0->CONTROLS[chn].CnSC &
			(TPM_CnSC_ELSA_SHIFT | TPM_CnSC_MSA_SHIFT | TPM_CnSC_ELSB_SHIFT | TPM_CnSC_MSB_SHIFT)));

}

void write_pwn_duty(uint8_t chn, uint8_t duty)
{
	uint32_t mod = 0;
	mod = TPM0->MOD;
	TPM0->CONTROLS[chn].CnV = (mod * duty)/100;
}

void delay()
{
	int i = 0;

	for(i=0; i < 1000000; i++)
		asm("nop");
}

int main()
{
	uint8_t i = 0;

	init_pwm();
	init_pwm_chn(GLED_PWM_CHN);
	init_pwm_chn(RLED_PWM_CHN);

	while(1) {
		//TODO: Set this correctly disable channel and then set it?
		for(i=0; i < 100; i += 5) {
			write_pwn_duty(RLED_PWM_CHN, 99 - i);
			write_pwn_duty(GLED_PWM_CHN, i);
			delay();
		}
	}
	return 1;
}
