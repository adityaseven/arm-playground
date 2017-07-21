/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  DAC to speaker, hopefully can recreate monotone sound
 *
 *        Version:  1.0
 *        Created:  07/20/2017 17:11:17
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Raghava Aditya Renukunta (), adi.raghava3@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <MKL46Z4.h>

#define DAC_PIN  30
#define DAC_PORT PORTE

#define SINE_LEN 256

extern uint8_t _binary_ring_b_start;
extern uint8_t _binary_ring_b_end;
extern uint8_t _binary_ring_b_size;


/* 16 bits Data Mode */
static const uint16_t sine_wave_form[SINE_LEN] = {
        2047U,  2097U,  2147U,  2198U,  2248U,  2298U,  2347U,  2397U,  2446U,  2496U,  2544U,  2593U,  2641U,  2689U,
        2737U,  2784U,  2830U,  2877U,  2922U,  2967U,  3012U,  3056U,  3099U,  3142U,  3184U,  3226U,  3266U,  3306U,
        3346U,  3384U,  3422U,  3458U,  3494U,  3530U,  3564U,  3597U,  3629U,  3661U,  3691U,  3721U,  3749U,  3776U,
        3803U,  3828U,  3852U,  3875U,  3897U,  3918U,  3938U,  3957U,  3974U,  3991U,  4006U,  4020U,  4033U,  4044U,
        4055U,  4064U,  4072U,  4079U,  4084U,  4088U,  4092U,  4093U,  4094U,  4093U,  4092U,  4088U,  4084U,  4079U,
        4072U,  4064U,  4055U,  4044U,  4033U,  4020U,  4006U,  3991U,  3974U,  3957U,  3938U,  3918U,  3897U,  3875U,
        3852U,  3828U,  3803U,  3776U,  3749U,  3721U,  3691U,  3661U,  3629U,  3597U,  3564U,  3530U,  3494U,  3458U,
        3422U,  3384U,  3346U,  3306U,  3266U,  3226U,  3184U,  3142U,  3099U,  3056U,  3012U,  2967U,  2922U,  2877U,
        2830U,  2784U,  2737U,  2689U,  2641U,  2593U,  2544U,  2496U,  2446U,  2397U,  2347U,  2298U,  2248U,  2198U,
        2147U,  2097U,  2047U,  1997U,  1947U,  1896U,  1846U,  1796U,  1747U,  1697U,  1648U,  1598U,  1550U,  1501U,
        1453U,  1405U,  1357U,  1310U,  1264U,  1217U,  1172U,  1127U,  1082U,  1038U,  995U,   952U,   910U,   868U,
        828U,   788U,   748U,   710U,   672U,   636U,   600U,   564U,   530U,   497U,   465U,   433U,   403U,   373U,
        345U,   318U,   291U,   266U,   242U,   219U,   197U,   176U,   156U,   137U,   120U,   103U,   88U,    74U,
        61U,    50U,    39U,    30U,    22U,    15U,    10U,    6U,     2U,     1U,     0U,     1U,     2U,     6U,
        10U,    15U,    22U,    30U,    39U,    50U,    61U,    74U,    88U,    103U,   120U,   137U,   156U,   176U,
        197U,   219U,   242U,   266U,   291U,   318U,   345U,   373U,   403U,   433U,   465U,   497U,   530U,   564U,
        600U,   636U,   672U,   710U,   748U,   788U,   828U,   868U,   910U,   952U,   995U,   1038U,  1082U,  1127U,
        1172U,  1217U,  1264U,  1310U,  1357U,  1405U,  1453U,  1501U,  1550U,  1598U,  1648U,  1697U,  1747U,  1796U,
        1846U,  1896U,  1947U,  1997U
};

static void dac_init()
{
	SIM->SCGC5 |= SIM_SCGC5_PORTE(1);
	SIM->SCGC6 |= SIM_SCGC6_DAC0(1);

	DAC_PORT->PCR[DAC_PIN]  &= ~PORT_PCR_MUX_MASK;
	DAC_PORT->PCR[DAC_PIN] |= PORT_PCR_MUX(0x0);

	DAC0->C0 = (DAC_C0_DACRFS(1) | DAC_C0_DACEN(1));
}

static void dac_write(uint16_t dac_val)
{
	DAC0->DAT[0].DATL = (uint8_t)(0x00FF & dac_val);
	DAC0->DAT[0].DATH = (uint8_t)((0xF00 & dac_val) >> 8);
}

static void delay(uint32_t dac_val)
{
	volatile unsigned int i;

	for (i = 0U; i < dac_val; i++) {
			__asm__("nop");
	}
}

int main()
{
	int i = 0;
	uint8_t *ring = &_binary_ring_b_start;

	dac_init();

	while(1) {

		for (i = 0; i < _binary_ring_b_size; i++) {
			dac_write(ring[i] << 3);
			//delay(ring[i]);
		}

	}


	return 0;
}

