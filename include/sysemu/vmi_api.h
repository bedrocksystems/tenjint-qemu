#ifndef VMI_API_H
#define VMI_API_H
#include "qemu/osdep.h"

#include <linux/kvm_vmi.h>

int vmi_api_init(void);
void vmi_api_uninit(void);

union kvm_vmi_event* vmi_api_get_event(void);
void vmi_api_wait_event(void);

#endif