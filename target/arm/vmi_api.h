/*
 * QEMU KVM VMI ARM API
 *
 * Copyright BedRock Systems, Inc. 2020
 *
 * Authors:
 *  Jonas Pfoh     <jonas@bedrocksystems.com>
 *  Sebastian Vogl <sebastian@bedrocksystems.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 *
 */
#ifndef ARM_VMI_API_H
#define ARM_VMI_API_H
#include "qemu/osdep.h"
#include "cpu.h"

CPUARMState* vmi_api_get_cpu_state(uint32_t cpu_num);

#endif
