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
#include "vmi_api.h"

#include "sysemu/kvm.h"
#include "sysemu/vmi_ioctl.h"


CPUX86State* vmi_api_get_cpu_state(uint32_t cpu_num) {
    CPUState *cpu = NULL;

    cpu = qemu_get_cpu(cpu_num);
    kvm_cpu_synchronize_state(cpu);
    return &(X86_CPU(cpu)->env);
}

int vmi_api_get_lbr_state(uint32_t cpu_num,
                          struct kvm_vmi_lbr_info *lbr_state) {
    CPUState *cpu = NULL;
    struct vmi_ioctl_data_t vmi_ioctl_data = {.data = lbr_state,
                                              .type = KVM_VMI_GET_LBR,
                                              .rv = 0};

    cpu = qemu_get_cpu(cpu_num);
    if (cpu == NULL)
        return -1;
    run_on_cpu(cpu, vmi_api_ioctl, RUN_ON_CPU_HOST_PTR(&vmi_ioctl_data));

    return vmi_ioctl_data.rv;
}
