#ifndef VMI_API_H
#define VMI_API_H
#include <asm/kvm_vmi.h>

#define VMI_ARCH_UNSUPPORTED    -1
#define VMI_ARCH_X86_64         0
#define VMI_ARCH_AARCH64        1

#define VMI_EVENT_KVM           0
#define VMI_EVENT_STOP          1
#define VMI_EVENT_VM_READY      2
#define VMI_EVENT_VM_SHUTDOWN   3

struct vmi_event {
    uint32_t type;
    int32_t arch;
    union {
        union kvm_vmi_event *kvm_vmi_event;
    };
};

extern int vmi_api_init(void);
extern void vmi_api_uninit(void);

extern struct vmi_event* vmi_api_get_event(void);
extern void vmi_api_wait_event(void);

#endif