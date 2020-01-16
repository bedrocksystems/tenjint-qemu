/*
 * QEMU KVM I386 VMI support
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
#include "qemu/osdep.h"
#include "cpu.h"

#include "sysemu/vmi_api.h"

uint64_t vmi_api_arch_vtop(uint64_t addr, uint64_t dtb) {
    X86CPU trans_cpu;
    CPUState *trans_cs = CPU(&trans_cpu);
    CPUState *cs = qemu_get_cpu(0);
    X86CPU *cpu = X86_CPU(cs);
    uint64_t rv;

    memcpy(&trans_cpu, cpu, sizeof(X86CPU));
    trans_cpu.env.cr[3] = dtb;

    rv = cpu_get_phys_page_debug(trans_cs, addr & TARGET_PAGE_MASK);
    if (rv != (uint64_t)-1)
        rv += addr & ~TARGET_PAGE_MASK;
    return rv;
}
