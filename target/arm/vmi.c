#include "qemu/osdep.h"
#include "cpu.h"

#include "sysemu/vmi_api.h"

uint64_t vmi_api_arch_vtop(uint64_t addr, uint64_t dtb) {
    ARMCPU trans_cpu;
    CPUState *trans_cs = CPU(&trans_cpu);
    CPUState *cs = qemu_get_cpu(0);
    ARMCPU *cpu = ARM_CPU(cs);
    uint64_t rv;

    memcpy(&trans_cpu, cpu, sizeof(ARMCPU));

    trans_cpu.env.cp15.ttbr0_el[1] = addr;
    // trans_cpu.env.cp15.ttbr1_el[1] = 
    // trans_cpu.env.cp15.tcr_el[1].raw_tcr = 
    // trans_cpu.env.cp15.tcr_el[1].mask = 
    // trans_cpu.env.cp15.tcr_el[1].base_mask = 

    printf("--------------------------------------------------------\n");
    printf("trans_cpu.env.cp15.ttbr0_el[1] = 0x%llx\n", trans_cpu.env.cp15.ttbr0_el[1]);
    printf("trans_cpu.env.cp15.ttbr1_el[1] = 0x%llx\n", trans_cpu.env.cp15.ttbr1_el[1]);
    printf("trans_cpu.env.cp15.tcr_el[1].raw_tcr = 0x%llx\n", trans_cpu.env.cp15.tcr_el[1].raw_tcr);
    printf("trans_cpu.env.cp15.tcr_el[1].mask = 0x%llx\n", trans_cpu.env.cp15.tcr_el[1].mask);
    printf("trans_cpu.env.cp15.tcr_el[1].base_mask\n", trans_cpu.env.cp15.tcr_el[1].base_mask);
    printf("--------------------------------------------------------\n");

    rv = cpu_get_phys_page_debug(trans_cs, addr & TARGET_PAGE_MASK);
    rv += addr & ~TARGET_PAGE_MASK;
    return rv;
}
