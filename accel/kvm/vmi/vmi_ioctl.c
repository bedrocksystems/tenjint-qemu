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
#include "sysemu/vmi_ioctl.h"
#include "sysemu/kvm.h"

void vmi_api_ioctl(CPUState *cpu, run_on_cpu_data data) {
    struct vmi_ioctl_data_t *vmi_ioctl_data =
        (struct vmi_ioctl_data_t*) data.host_ptr;

    vmi_ioctl_data->rv = kvm_vcpu_ioctl(cpu, vmi_ioctl_data->type,
                                        vmi_ioctl_data->data);
}
