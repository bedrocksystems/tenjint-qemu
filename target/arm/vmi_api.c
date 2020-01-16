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
#include "vmi_api.h"

#include "sysemu/kvm.h"


CPUARMState* vmi_api_get_cpu_state(uint32_t cpu_num) {
    CPUState *cpu = NULL;

    cpu = qemu_get_cpu(cpu_num);
    kvm_cpu_synchronize_state(cpu);
    return &(ARM_CPU(cpu)->env);
}
