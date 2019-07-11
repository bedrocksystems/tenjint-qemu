#ifndef VMI_EVENT_H
#define VMI_EVENT_H
#include "qemu/osdep.h"

#include <linux/kvm_vmi.h>

void vmi_put_event(union kvm_vmi_event *event, bool needs_free);

#endif