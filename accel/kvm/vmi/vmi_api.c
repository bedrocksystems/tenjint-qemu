#include "qemu/osdep.h"

#include "sysemu/cpus.h"
#include "sysemu/vmi_api.h"
#include "qemu/main-loop.h"

#include "vmi.h"


int vmi_api_init(void) {
    qemu_mutex_lock_iothread();
    return 0;
}

void vmi_api_uninit(void) {
    qemu_mutex_unlock_iothread();
    return;
}

union kvm_vmi_event* vmi_api_get_event(void) {
    return vmi_get_event();
}

void vmi_api_wait_event(void) {
    vmi_wait_event();
}