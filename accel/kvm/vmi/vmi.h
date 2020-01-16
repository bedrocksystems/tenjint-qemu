/*
 * QEMU KVM VMI support
 *
 * Copyright BedRock Systems, Inc. 2020
 *
 * Authors:
 *  Jonas Pfoh     <jonas@bedrocksystems.com>
 *  Sebastian Vogl <sebastian@bedrocksystems.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 *
 */

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
int vmi_start_vm(void);
int vmi_wait_event(time_t secs);
void vmi_stop_vm(void);
void vmi_wait_init(void);
#endif