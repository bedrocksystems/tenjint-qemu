#include "qemu/osdep.h"

#include <linux/kvm_vmi.h>

#include "sysemu/cpus.h"
#include "qom/cpu.h"
#include "sysemu/kvm.h"
#include "sysemu/vmi_api.h"
#include "qemu/main-loop.h"

#include "vmi.h"


int vmi_api_init(void) {
    qemu_mutex_lock_iothread();
    vmi_wait_init();
    return 0;
}

void vmi_api_uninit(void) {
    qemu_mutex_unlock_iothread();
    return;
}

struct vmi_event* vmi_api_get_event(void) {
    return vmi_get_event();
}

void vmi_api_wait_event(void) {
    vmi_wait_event();
}

int vmi_api_feature_update_all(union kvm_vmi_feature *feature) {
    int rv = 0;
    CPUState *cpu = NULL;

    CPU_FOREACH(cpu) {
        rv = kvm_vcpu_ioctl(cpu, KVM_VMI_FEATURE_UPDATE, feature);
        if (rv < 0) {
            break;
        }
    }

    return rv;
}

int vmi_api_feature_update_single(uint32_t cpu_num, union kvm_vmi_feature *feature) {
    int rv = 0;
    CPUState *cpu = NULL;

    cpu = qemu_get_cpu(cpu_num);
    if (cpu == NULL)
        return -1;
    rv = kvm_vcpu_ioctl(cpu, KVM_VMI_FEATURE_UPDATE, feature);

    return rv;
}