#include "qemu/osdep.h"

#include <linux/kvm_vmi.h>

#include "sysemu/cpus.h"
#include "qom/cpu.h"
#include "sysemu/kvm.h"
#include "sysemu/vmi_api.h"
#include "qemu/main-loop.h"
#include "exec/cpu-common.h"

#include "vmi.h"


int vmi_api_init(void) {
    qemu_mutex_lock_iothread();
    vmi_wait_init();
    return 0;
}

void vmi_api_uninit(void) {
    qemu_mutex_unlock_iothread();
}

void vmi_api_request_stop(void) {
    vmi_request_stop();
}

struct vmi_event* vmi_api_get_event(void) {
    return vmi_get_event();
}

int vmi_api_wait_event(time_t secs) {
    return vmi_wait_event(secs);
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

int vmi_api_feature_update_single(uint32_t cpu_num,
                                  union kvm_vmi_feature *feature) {
    int rv = 0;
    CPUState *cpu = NULL;

    cpu = qemu_get_cpu(cpu_num);
    if (cpu == NULL)
        return -1;
    rv = kvm_vcpu_ioctl(cpu, KVM_VMI_FEATURE_UPDATE, feature);

    return rv;
}

int vmi_api_slp_update_all(struct kvm_vmi_slp_perm *slp_perm) {
    int rv = 0;
    CPUState *cpu = NULL;

    CPU_FOREACH(cpu) {
        rv = kvm_vcpu_ioctl(cpu, KVM_VMI_SET_SLP, slp_perm);
        if (rv < 0) {
            break;
        }
    }

    return rv;
}

int vmi_api_slp_update_single(uint32_t cpu_num,
                              struct kvm_vmi_slp_perm *slp_perm) {
    int rv = 0;
    CPUState *cpu = NULL;

    cpu = qemu_get_cpu(cpu_num);
    if (cpu == NULL)
        return -1;
    rv = kvm_vcpu_ioctl(cpu, KVM_VMI_SET_SLP, slp_perm);

    return rv;
}

uint64_t vmi_api_get_num_cpus(void) {
    CPUState *cpu;
    uint64_t rv = 0;

    CPU_FOREACH(cpu) {
        rv++;
    }
    return rv;
}

uint64_t vmi_api_get_ram_size(void) {
    return (uint64_t) ram_size;
}

int vmi_api_read_phys_mem(uint64_t addr, void *buf, uint64_t len) {
    cpu_physical_memory_read(addr, buf, len);
    return 0;
}

int vmi_api_write_phys_mem(uint64_t addr, const void *buf, uint64_t len) {
    cpu_physical_memory_write(addr, buf, len);
    return 0;
}

uint32_t vmi_api_get_page_bits(void) {
    return TARGET_PAGE_BITS;
}

uint32_t vmi_api_get_page_size(void) {
    return TARGET_PAGE_SIZE;
}
