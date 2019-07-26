#ifndef VMI_H
#define VMI_H

#include "qemu/typedefs.h"
#include "sysemu/accel.h"
#include "sysemu/vmi_api.h"

// vmi.c
int vmi_init(MachineState *ms);
void vmi_setup_post(MachineState *ms, AccelState *accel);
void vmi_uninit(MachineState *ms, AccelState *accel);

// vmi_event.c
int vmi_event_init(MachineState *ms);
void vmi_event_uninit(MachineState *ms, AccelState *accel);
void vmi_request_stop(void);
struct vmi_event* vmi_get_event(void);
void vmi_wait_event(void);
void vmi_wait_init(void);
#endif