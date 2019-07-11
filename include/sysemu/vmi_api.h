#ifndef VMI_API_H
#define VMI_API_H
#include <linux/kvm_vmi.h>

extern int vmi_api_init(void);
extern void vmi_api_uninit(void);

extern union kvm_vmi_event* vmi_api_get_event(void);
extern void vmi_api_wait_event(void);

#endif