#include "qemu/osdep.h"
#include "cpu.h"

#include "sysemu/vmi_api.h"

uint64_t vmi_api_arch_vtop(uint64_t addr, uint64_t dtb) {
    X86CPU trans_cpu;
    CPUState *trans_cs = CPU(&trans_cpu);
    CPUState *cs = qemu_get_cpu(0);
    X86CPU *cpu = X86_CPU(cs);
    uint64_t rv;

    memcpy(&trans_cpu, cpu, sizeof(X86CPU));
    trans_cpu.env.cr[3] = dtb;

    rv = cpu_get_phys_page_debug(trans_cs, addr & TARGET_PAGE_MASK);
    if (rv != (uint64_t)-1)
        rv += addr & ~TARGET_PAGE_MASK;
    return rv;
}
