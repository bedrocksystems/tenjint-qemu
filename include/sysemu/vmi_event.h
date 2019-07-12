#ifndef VMI_EVENT_H
#define VMI_EVENT_H

#include "sysemu/vmi_api.h"

#include <asm/kvm_vmi.h>

void vmi_put_kvm_event(union kvm_vmi_event *event);
void vmi_put_event(struct vmi_event *event);

#endif