#ifndef I386_VMI_API_H
#define I386_VMI_API_H
#include "qemu/osdep.h"
#include "cpu.h"

CPUX86State* vmi_api_get_cpu_state(uint32_t cpu_num);

#endif
