/*
 * QEMU KVM VMI IOCTLs
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
#ifndef VMI_IOCTL_H
#define VMI_IOCTL_H

#include "qemu/osdep.h"

#include "sysemu/cpus.h"
#include "qom/cpu.h"

struct vmi_ioctl_data_t {
    void *data;
    int type;
    int rv;
};

void vmi_api_ioctl(CPUState *cpu, run_on_cpu_data data);

#endif
