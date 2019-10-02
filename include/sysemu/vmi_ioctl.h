#ifndef VMI_IOCTL_H
#define VMI_IOCTL_H

#include "qemu/osdep.h"

#include <linux/kvm_vmi.h>

#include "sysemu/cpus.h"
#include "qom/cpu.h"

struct vmi_ioctl_data_t {
    union {
        void *data;
        union kvm_vmi_feature *feature;
        struct kvm_vmi_slp_perm *slp_perm;
    };
    int type;
    int rv;
};

void vmi_api_ioctl(CPUState *cpu, run_on_cpu_data data);

#endif
