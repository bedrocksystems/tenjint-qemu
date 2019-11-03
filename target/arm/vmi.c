#include "qemu/osdep.h"
#include "cpu.h"

#include "sysemu/vmi_api.h"

// this returns (uint64_t)-1 on failure
uint64_t vmi_api_arch_vtop(uint64_t addr, uint64_t dtb) {
    ARMCPU trans_cpu;
    CPUState *trans_cs = CPU(&trans_cpu);
    CPUState *cs = qemu_get_cpu(0);
    ARMCPU *cpu = ARM_CPU(cs);
    uint64_t rv, tcr;

    memcpy(&trans_cpu, cpu, sizeof(ARMCPU));

    // printf("--------------------------------------------------------\n");
    // printf("trans_cpu.env.cp15.ttbr0_el[1] = 0x%lx\n", trans_cpu.env.cp15.ttbr0_el[1]);
    // printf("trans_cpu.env.cp15.ttbr1_el[1] = 0x%lx\n", trans_cpu.env.cp15.ttbr1_el[1]);
    // printf("trans_cpu.env.cp15.tcr_el[1].raw_tcr = 0x%lx\n", trans_cpu.env.cp15.tcr_el[1].raw_tcr);
    // printf("trans_cpu.env.cp15.tcr_el[1].mask = 0x%x\n", trans_cpu.env.cp15.tcr_el[1].mask);
    // printf("trans_cpu.env.cp15.tcr_el[1].base_mask = 0x%x\n", trans_cpu.env.cp15.tcr_el[1].base_mask);
    // printf("--------------------------------------------------------\n");

    tcr = trans_cpu.env.cp15.tcr_el[1].raw_tcr;

    trans_cpu.env.cp15.ttbr0_el[1] = dtb;
    trans_cpu.env.cp15.ttbr1_el[1] = 0;
    trans_cpu.env.cp15.tcr_el[1].raw_tcr = tcr & 0xffffffffffffffc0;
    trans_cpu.env.cp15.tcr_el[1].mask = 0;
    trans_cpu.env.cp15.tcr_el[1].base_mask = 0;

    rv = cpu_get_phys_page_debug(trans_cs, addr & TARGET_PAGE_MASK);
    if (rv != (uint64_t)-1)
        rv += addr & ~TARGET_PAGE_MASK;
    return rv;
}
