#include "qemu/osdep.h"

#include "qemu/queue.h"
#include "qemu/thread.h"
#include "qemu/main-loop.h"
#include "sysemu/cpus.h"
#include "sysemu/vmi_event.h"
#include "sysemu/vmi_api.h"
#include "sysemu/sysemu.h"

#include "vmi.h"

struct vmi_event_entry {
    struct vmi_event *event;
    QSIMPLEQ_ENTRY(vmi_event_entry) entry;
};

QSIMPLEQ_HEAD(vmi_event_queue_t, vmi_event_entry);

static struct vmi_event_queue_t *vmi_event_queue = NULL;
static struct vmi_event_queue_t *vmi_free_queue = NULL;

static QemuCond vmi_event_cv;

int vmi_event_init(MachineState *ms){
    vmi_event_queue = g_malloc0(sizeof(struct vmi_event_queue_t));
    QSIMPLEQ_INIT(vmi_event_queue);

    vmi_free_queue = g_malloc0(sizeof(struct vmi_event_queue_t));
    QSIMPLEQ_INIT(vmi_free_queue);

    qemu_cond_init(&vmi_event_cv);

    return 0;
}

void vmi_event_uninit(MachineState *ms, AccelState *accel){
    struct vmi_event_entry *i = NULL;
    struct vmi_event_entry *tmp = NULL;

    QSIMPLEQ_FOREACH_SAFE(i, vmi_event_queue, entry, tmp) {
        g_free(i->event);
        g_free(i);
    }
    g_free(vmi_event_queue);
    vmi_event_queue = NULL;

    QSIMPLEQ_FOREACH_SAFE(i, vmi_free_queue, entry, tmp) {
        g_free(i->event);
        g_free(i);
    }
    g_free(vmi_free_queue);
    vmi_free_queue = NULL;
}

void vmi_put_event(struct vmi_event *event){
    struct vmi_event_entry *e = NULL;

#if defined(TARGET_X86_64)
    event->arch = VMI_ARCH_X86;
#elif defined(TARGET_AARCH64)
    event->arch = VMI_ARCH_AARCH64;
#elif defined(TARGET_I386) || defined(TARGET_ARM)
    event->arch = VMI_ARCH_UNSUPPORTED;
#else
#error Invalid target arch
#endif

    e = g_malloc0(sizeof(struct vmi_event_entry));
    e->event = event;

    QSIMPLEQ_INSERT_TAIL(vmi_event_queue, e, entry);

    pause_all_vcpus_signal();

    qemu_cond_broadcast(&vmi_event_cv);
}

void vmi_put_kvm_event(union kvm_vmi_event *event){
    struct vmi_event *e = NULL;

    e = g_malloc0(sizeof(struct vmi_event));
    e->type = VMI_EVENT_KVM;
    e->kvm_vmi_event = event;

    vmi_put_event(e);
}

struct vmi_event* vmi_get_event(void){
    struct vmi_event_entry *e = NULL;
    struct vmi_event *rv = NULL;

    e = QSIMPLEQ_FIRST(vmi_event_queue);
    if (e) {
        QSIMPLEQ_REMOVE_HEAD(vmi_event_queue, entry);
        QSIMPLEQ_INSERT_TAIL(vmi_free_queue, e, entry);
        rv = e->event;
    }
    return rv;
}

void vmi_wait_event(void){
    struct vmi_event_entry *i = NULL;
    struct vmi_event_entry *tmp = NULL;

    QSIMPLEQ_FOREACH_SAFE(i, vmi_free_queue, entry, tmp) {
        g_free(i->event);
        g_free(i);
    }
    QSIMPLEQ_INIT(vmi_free_queue);

    while(QSIMPLEQ_EMPTY_ATOMIC(vmi_event_queue)) {
        if (!runstate_is_running()){
            vm_start();
        }
        else{
            resume_all_vcpus();
        }
        qemu_mutex_wait_iothread(&vmi_event_cv);
    }

    pause_all_vcpus_wait();
}


