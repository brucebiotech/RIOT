/*
 * Copyright (C) 2025
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Blinky application -  secure state boot
 *
 * @author      Gregor Bruce
 *
 * @}
 */
#include "board.h"
#include <stdint.h>

#define TZM_IS_NONSECURE_CALLED __attribute__((cmse_nonsecure_call))
typedef void (*funcptr_ns)(void) TZM_IS_NONSECURE_CALLED;

//
// this function never returns
//
void
start_non_secure_state (void) {
	uint32_t address = (uint32_t) NONSECURE_STATE_VTOR_ADDRESS;
	funcptr_ns jump;
	
	__TZ_set_MSP_NS(*((uint32_t *)(address)));
	
	SCB_NS->VTOR = address;
	jump  = (funcptr_ns)(*((uint32_t *)((SCB_NS->VTOR) + 4U)));

	jump ();
}

#define SERIAL0_IRQn UARTE0_SPIM0_SPIS0_TWIM0_TWIS0_IRQn
#define SERIAL1_IRQn UARTE1_SPIM1_SPIS1_TWIM1_TWIS1_IRQn
#define SERIAL2_IRQn UARTE2_SPIM2_SPIS2_TWIM2_TWIS2_IRQn
#define SERIAL3_IRQn UARTE3_SPIM3_SPIS3_TWIM3_TWIS3_IRQn

void setup_trustzone (void) {

	SAU->CTRL |= (1 << SAU_CTRL_ALLNS_Pos);
	
	#if ((__FPU_PRESENT == 1) && (__FPU_USED == 1))
	SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));    /* set CP10, CP11 Full Access in Secure mode */
	#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
	SCB_NS->CPACR |= ((3UL << 10*2) | (3UL << 11*2));    /* set CP10, CP11 Full Access in Non-secure mode */
	#endif /* (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */
	#endif /* ((__FPU_PRESENT == 1) && (__FPU_USED == 1)) */

	SCB->NSACR |= ((3UL << 10));   /* enable CP10, CP11 Non-secure Access */
	
	// i think this makes the gpio non-secure
	NRF_PERIPHERAL(NRF_SPU)->GPIOPORT[0].PERM = 0;

	//
	// make selected peripherals non-secure
	//
	uint32_t non_secure = ~(
		(1 << SPU_PERIPHID_PERM_SECATTR_Pos)
	);

	NRF_PERIPHERAL(NRF_SPU)->PERIPHID[RTC0_IRQn].PERM &= non_secure;
	NRF_PERIPHERAL(NRF_SPU)->PERIPHID[IPC_IRQn].PERM &= non_secure;
	NRF_PERIPHERAL(NRF_SPU)->PERIPHID[FPU_IRQn].PERM &= non_secure;
	NRF_PERIPHERAL(NRF_SPU)->PERIPHID[CLOCK_POWER_IRQn].PERM &= non_secure;
	NRF_PERIPHERAL(NRF_SPU)->PERIPHID[SERIAL0_IRQn].PERM &= non_secure;
	NRF_PERIPHERAL(NRF_SPU)->PERIPHID[SERIAL1_IRQn].PERM &= non_secure;
	NRF_PERIPHERAL(NRF_SPU)->PERIPHID[SERIAL2_IRQn].PERM &= non_secure;
	NRF_PERIPHERAL(NRF_SPU)->PERIPHID[SERIAL3_IRQn].PERM &= non_secure;

	NRF_PERIPHERAL(NRF_SPU)->PERIPHID[66].PERM &= non_secure;

	//
	// FLASH 32, 32k regions: 
	// 	first 8 are secure
	// 	remaining are non-secure
	for (int i = 8; i < 32; i++) {
		NRF_PERIPHERAL(NRF_SPU)->FLASHREGION[i].PERM = (
				(SPU_FLASHREGION_PERM_SECATTR_Non_Secure << SPU_FLASHREGION_PERM_SECATTR_Pos)
			|	(SPU_FLASHREGION_PERM_READ_Enable << SPU_FLASHREGION_PERM_READ_Pos)
			|	(SPU_FLASHREGION_PERM_WRITE_Enable << SPU_FLASHREGION_PERM_WRITE_Pos)
			|	(SPU_FLASHREGION_PERM_EXECUTE_Enable << SPU_FLASHREGION_PERM_EXECUTE_Pos)
		);
	}

	//
	// RAM 32, 8k regions: 
	// 
	for (int i = 0; i < 32; i++) {
		NRF_PERIPHERAL(NRF_SPU)->RAMREGION[i].PERM = (
				(SPU_RAMREGION_PERM_SECATTR_Non_Secure << SPU_RAMREGION_PERM_SECATTR_Pos)
			|	(SPU_RAMREGION_PERM_READ_Enable << SPU_RAMREGION_PERM_READ_Pos)
			|	(SPU_RAMREGION_PERM_WRITE_Enable << SPU_RAMREGION_PERM_WRITE_Pos)
			|	(SPU_RAMREGION_PERM_EXECUTE_Enable << SPU_RAMREGION_PERM_EXECUTE_Pos)
		);
	}

	//
	// for IRQ's < 32
	//
	NVIC->ITNS[0] = (
			(1 << CLOCK_POWER_IRQn)
		|	(1 << SERIAL0_IRQn)
		|	(1 << SERIAL1_IRQn)
		|	(1 << SERIAL2_IRQn)
		|	(1 << SERIAL3_IRQn)
		|	(1 << GPIOTE0_IRQn)
		|	(1 << RTC0_IRQn)
	);
	
	//
	// for IRQ's < 64
	//
	NVIC->ITNS[1] = (
			(1 << (IPC_IRQn - 32))
		|	(1 << (FPU_IRQn - 32))
		|	(1 << (GPIOTE1_IRQn - 32))
	);

	NRF_PERIPHERAL(NRF_SPU)->DPPI[0].PERM = (
		0	// all insecure
	);

	//
	// we can have 2 regions in secure flash that are callable from 
	// non-secure flash
	//
	NRF_PERIPHERAL(NRF_SPU)->FLASHNSC[0].REGION = 0;
	NRF_PERIPHERAL(NRF_SPU)->FLASHNSC[0].SIZE = 0;
	NRF_PERIPHERAL(NRF_SPU)->FLASHNSC[1].REGION = 0;
	NRF_PERIPHERAL(NRF_SPU)->FLASHNSC[1].SIZE = 0;

	NRF_PERIPHERAL(NRF_SPU)->RAMNSC[0].REGION = 0;
	NRF_PERIPHERAL(NRF_SPU)->RAMNSC[0].SIZE = 0;
	NRF_PERIPHERAL(NRF_SPU)->RAMNSC[1].REGION = 0;
	NRF_PERIPHERAL(NRF_SPU)->RAMNSC[1].SIZE = 0;
}

int main (void) {

	//
	// This codes enables debug access, if disabled. The nrf91xx disables 
	// debug access by default.
	//
	if ((NRF_PERIPHERAL (NRF_UICR)->APPROTECT) != 0x50FA50FA) {

		NRF_PERIPHERAL (NRF_NVMC)->CONFIG = NVMC_CONFIG_WEN_Wen;
		while (NRF_PERIPHERAL (NRF_NVMC)->READY == NVMC_READY_READY_Busy){}

		NRF_PERIPHERAL (NRF_UICR)->APPROTECT = 0x50FA50FA;
		
		NRF_PERIPHERAL (NRF_NVMC)->CONFIG = NVMC_CONFIG_WEN_Ren;
		while (NRF_PERIPHERAL (NRF_NVMC)->READY == NVMC_READY_READY_Busy){}
	}
	
	setup_trustzone ();
	start_non_secure_state ();
	
	// execution never gets here

	return 0;
}
