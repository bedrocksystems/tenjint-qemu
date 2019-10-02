#include "sysemu/vmi_ioctl.h"
#include "sysemu/kvm.h"

void vmi_api_ioctl(CPUState *cpu, run_on_cpu_data data) {
    struct vmi_ioctl_data_t *vmi_ioctl_data =
        (struct vmi_ioctl_data_t*) data.host_ptr;

    vmi_ioctl_data->rv = kvm_vcpu_ioctl(cpu, vmi_ioctl_data->type,
                                        vmi_ioctl_data->data);
}
