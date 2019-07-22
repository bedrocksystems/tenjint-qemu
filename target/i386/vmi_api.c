#include "vmi_api.h"

#include "sysemu/kvm.h"


CPUX86State* vmi_api_get_cpu_state(uint32_t cpu_num) {
    CPUState *cpu = NULL;

    cpu = qemu_get_cpu(cpu_num);
    kvm_cpu_synchronize_state(cpu);
    return &(X86_CPU(cpu)->env);
}
