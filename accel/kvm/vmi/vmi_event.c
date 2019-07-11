#include "qemu/osdep.h"

#include "qemu/queue.h"
#include "qemu/thread.h"
#include "qemu/main-loop.h"
#include "sysemu/cpus.h"
#include "sysemu/vmi_event.h"

#include "vmi.h"

struct vmi_event {
    union kvm_vmi_event *event;
    bool needs_free;
    QSIMPLEQ_ENTRY(vmi_event) entry;
};

QSIMPLEQ_HEAD(vmi_event_queue_t, vmi_event);

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
    struct vmi_event *i = NULL;
    struct vmi_event *tmp = NULL;

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

void vmi_put_event(union kvm_vmi_event *event, bool needs_free){
    struct vmi_event *e = NULL;

    e = g_malloc0(sizeof(struct vmi_event));
    e->event = event;
    e->needs_free = needs_free;

    QSIMPLEQ_INSERT_TAIL(vmi_event_queue, e, entry);

    pause_all_vcpus_signal();
}

union kvm_vmi_event* vmi_get_event(void){
    struct vmi_event *e = NULL;
    union kvm_vmi_event *rv = NULL;

    e = QSIMPLEQ_FIRST(vmi_event_queue);
    if (e) {
        QSIMPLEQ_REMOVE_HEAD(vmi_event_queue, entry);
        rv = e->event;
        if (e->needs_free) {
            QSIMPLEQ_INSERT_TAIL(vmi_free_queue, e, entry);
        }
        else {
            g_free(e);
        }
    }
    return rv;
}

void vmi_wait_event(void){
    struct vmi_event *i = NULL;
    struct vmi_event *tmp = NULL;

    QSIMPLEQ_FOREACH_SAFE(i, vmi_free_queue, entry, tmp) {
        g_free(i->event);
        g_free(i);
    }
    QSIMPLEQ_INIT(vmi_free_queue);

    while(QSIMPLEQ_EMPTY_ATOMIC(vmi_event_queue)) {
        qemu_mutex_wait_iothread(&vmi_event_cv);
    }

    pause_all_vcpus_wait();
}


