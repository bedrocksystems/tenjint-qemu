/*
 * QEMU KVM VMI Events
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
#ifndef VMI_EVENT_H
#define VMI_EVENT_H

#include "sysemu/vmi_api.h"

#include <asm/kvm_vmi.h>

void vmi_put_kvm_event(union kvm_vmi_event *event);
void vmi_put_event(struct vmi_event *event);

#endif