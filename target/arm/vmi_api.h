#ifndef ARM_VMI_API_H
#define ARM_VMI_API_H
#include "qemu/osdep.h"
#include "cpu.h"

CPUARMState* vmi_api_get_cpu_state(uint32_t cpu_num);

#endif
