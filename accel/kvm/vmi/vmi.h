#ifndef VMI_H
#define VMI_H

#include "qemu/typedefs.h"
#include "sysemu/accel.h"

void vmi_init(MachineState *ms);
void vmi_setup_post(MachineState *ms, AccelState *accel);
void vmi_uninit(MachineState *ms, AccelState *accel);
#endif