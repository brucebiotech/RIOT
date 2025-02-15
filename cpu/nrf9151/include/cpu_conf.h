/*
 * Copyright (C) 2025
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup        cpu_nrf9151 Nordic nRF9151 MCU
 * @ingroup         cpu
 * @brief           Nordic nRF91xx family of CPUs
 * @{
 *
 * @file
 * @brief       nRF9151 specific CPU configuration
 *
 * @author      
 *
 */
#ifndef CPU_CONF_H
#define CPU_CONF_H

/*
 * for this idea for trustzone to work, ALL references to nrf peripherals
 * need to use the NRF_PERIPHERAL() macro
 *
 * so this includes the nrf52 common code
 *
 */
#if defined (TRUSTZONE)
# if TRUSTZONE == TRUSTZONE_SECURE_STATE
#  define NRF_PERIPHERAL(P) P##_S
# elif TRUSTZONE == TRUSTZONE_NONSECURE_STATE
#  define NRF_PERIPHERAL(P) P##_NS
# endif
#endif

#ifndef NRF_PERIPHERAL
# error "trust zone not setup properly"
#endif

#include "vendor/nrf9160.h"
#include "vendor/nrf9160_bitfields.h"
#include "vendor/nrf9160_peripherals.h"

#include "cpu_conf_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    ARM Cortex-M specific CPU configuration
 * @{
 */
#define CPU_DEFAULT_IRQ_PRIO            (1U)    /**< Default ARM IRQ priority */
#define CPU_FLASH_BASE                  (0x00000000) /**< ROM Base Address */

#define CPU_IRQ_NUMOF                   (65U)   /**< nRF9160 specific IRQ count */
/** @} */

/**
 * @name   Flash page configuration
 * @{
 */
#define FLASHPAGE_SIZE              (4096U) /**< Size of a page in bytes */
#define FLASHPAGE_NUMOF             (256U)  /**< Total number of flash pages */

/* The minimum block size which can be written is 4B. However, the erase
 * block is always FLASHPAGE_SIZE.
 */
#define FLASHPAGE_WRITE_BLOCK_SIZE      (4U) /**< Minimum block size */
/* Writing should be always 4 bytes aligned */
#define FLASHPAGE_WRITE_BLOCK_ALIGNMENT (4U) /**< Mandatory alignment */
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* CPU_CONF_H */
/** @} */
