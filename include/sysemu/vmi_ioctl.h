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
