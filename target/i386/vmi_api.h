/*
 * QEMU KVM VMI I386 API
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
#ifndef I386_VMI_API_H
#define I386_VMI_API_H
#include "qemu/osdep.h"

#include <asm/kvm_vmi.h>

#include "cpu.h"

CPUX86State* vmi_api_get_cpu_state(uint32_t cpu_num);
int vmi_api_get_lbr_state(uint32_t cpu_num,
                          struct kvm_vmi_lbr_info *lbr_state);

#endif
