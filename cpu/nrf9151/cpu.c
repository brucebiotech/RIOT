/*
 * Copyright (C) 2025
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     cpu_nrf9151
 * @{
 *
 * @file
 * @brief       Implementation of the CPU initialization
 *
 * @author      Gregor Bruce
 *
 * @}
 */

#include "cpu.h"
#include "kernel_init.h"
#include "nrf_clock.h"
#include "periph_conf.h"
#include "periph/init.h"
#include "stdio_base.h"

/**
 * @brief    LFCLK Clock selection configuration guard
*/
#if ((CLOCK_LFCLK != CLOCK_LFCLKSRC_SRC_LFRC) && \
     (CLOCK_LFCLK != CLOCK_LFCLKSRC_SRC_LFXO))
#error "LFCLK init: CLOCK_LFCLK has invalid value"
#endif

#if TRUSTZONE == TRUSTZONE_SECURE_STATE
void enable_debug_access (void) {
	if ((NRF_PERIPHERAL (NRF_UICR)->APPROTECT) != 0x50FA50FA) {

		NRF_PERIPHERAL (NRF_NVMC)->CONFIG = NVMC_CONFIG_WEN_Wen;
		while (NRF_PERIPHERAL (NRF_NVMC)->READY == NVMC_READY_READY_Busy){}

		NRF_PERIPHERAL (NRF_UICR)->APPROTECT = 0x50FA50FA;
		
		NRF_PERIPHERAL (NRF_NVMC)->CONFIG = NVMC_CONFIG_WEN_Ren;
		while (NRF_PERIPHERAL (NRF_NVMC)->READY == NVMC_READY_READY_Busy){}
	}
}
#endif

/**
 * @brief   Initialize the CPU, set IRQ priorities
 */
void cpu_init(void)
{
#if TRUSTZONE == TRUSTZONE_SECURE_STATE
    clock_init_hf();
    enable_debug_access ();
#endif

    cortexm_init();

// these only if non-secure?

    /* initialize stdio */
    early_init();

    periph_init();
}
