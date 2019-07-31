#include "vmi_api.h"

#include "sysemu/kvm.h"


CPUARMState* vmi_api_get_cpu_state(uint32_t cpu_num) {
    CPUState *cpu = NULL;

    cpu = qemu_get_cpu(cpu_num);
    kvm_cpu_synchronize_state(cpu);
    return &(ARM_CPU(cpu)->env);
}
