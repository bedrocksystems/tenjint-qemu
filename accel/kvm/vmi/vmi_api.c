#include "qemu/osdep.h"

#include <linux/kvm_vmi.h>

#include "sysemu/cpus.h"
#include "qom/cpu.h"
#include "sysemu/kvm.h"
#include "sysemu/vmi_api.h"
#include "sysemu/vmi_ioctl.h"
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
    CPUState *cpu = NULL;
    struct vmi_ioctl_data_t vmi_ioctl_data = {.data = feature,
                                              .type = KVM_VMI_FEATURE_UPDATE,
                                              .rv = 0};

    CPU_FOREACH(cpu) {
        run_on_cpu(cpu, vmi_api_ioctl, RUN_ON_CPU_HOST_PTR(&vmi_ioctl_data));
        if (vmi_ioctl_data.rv < 0) {
            break;
        }
    }

    return vmi_ioctl_data.rv;
}

int vmi_api_feature_update_single(uint32_t cpu_num,
                                  union kvm_vmi_feature *feature) {
    CPUState *cpu = NULL;
    struct vmi_ioctl_data_t vmi_ioctl_data = {.data = feature,
                                              .type = KVM_VMI_FEATURE_UPDATE,
                                              .rv = 0};

    cpu = qemu_get_cpu(cpu_num);
    if (cpu == NULL)
        return -1;
    run_on_cpu(cpu, vmi_api_ioctl, RUN_ON_CPU_HOST_PTR(&vmi_ioctl_data));

    return vmi_ioctl_data.rv;
}

int vmi_api_slp_update_all(struct kvm_vmi_slp_perm *slp_perm) {
    CPUState *cpu = NULL;
    struct vmi_ioctl_data_t vmi_ioctl_data = {.data = slp_perm,
                                              .type = KVM_VMI_SET_SLP,
                                              .rv = 0};

    CPU_FOREACH(cpu) {
        run_on_cpu(cpu, vmi_api_ioctl, RUN_ON_CPU_HOST_PTR(&vmi_ioctl_data));
        if (vmi_ioctl_data.rv < 0) {
            break;
        }
    }

    return vmi_ioctl_data.rv;
}

int vmi_api_slp_update_single(uint32_t cpu_num,
                              struct kvm_vmi_slp_perm *slp_perm) {
    CPUState *cpu = NULL;
    struct vmi_ioctl_data_t vmi_ioctl_data = {.data = slp_perm,
                                              .type = KVM_VMI_SET_SLP,
                                              .rv = 0};

    cpu = qemu_get_cpu(cpu_num);
    if (cpu == NULL)
        return -1;
    run_on_cpu(cpu, vmi_api_ioctl, RUN_ON_CPU_HOST_PTR(&vmi_ioctl_data));

    return vmi_ioctl_data.rv;
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
