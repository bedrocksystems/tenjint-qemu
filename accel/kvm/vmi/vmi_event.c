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

#include "qemu/osdep.h"

#include "qemu/queue.h"
#include "qemu/thread.h"
#include "qemu/main-loop.h"
#include "sysemu/cpus.h"
#include "sysemu/vmi_event.h"
#include "sysemu/vmi_api.h"
#include "sysemu/sysemu.h"
#include "sysemu/kvm.h"
#include "ui/console.h"

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

void vmi_request_stop(void) {
    struct vmi_event *event = NULL;
    bool need_lock = false;

    event = g_malloc0(sizeof(struct vmi_event));
    event->type = VMI_EVENT_STOP;

    if (!qemu_mutex_iothread_locked()) {
        qemu_mutex_lock_iothread();
        need_lock = true;
    }
    vmi_put_event(event);
    if (need_lock) {
        qemu_mutex_unlock_iothread();
    }
}

void vmi_put_event(struct vmi_event *event){
    struct vmi_event_entry *e = NULL;

#if defined(TARGET_X86_64)
    event->arch = VMI_ARCH_X86_64;
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

    cpu_disable_ticks();
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

int vmi_start_vm(void) {
    struct vmi_event_entry *i = NULL;
    struct vmi_event_entry *tmp = NULL;
    CPUState *cpu = NULL;
    CPUState *resume_cpu = NULL;

    QSIMPLEQ_FOREACH_SAFE(i, vmi_free_queue, entry, tmp) {
        g_free(i->event);
        g_free(i);
    }
    QSIMPLEQ_INIT(vmi_free_queue);

    if (!QSIMPLEQ_EMPTY(vmi_event_queue)) {
        vmi_stop_vm();
        return 0;
    }

#if defined(TARGET_AARCH64) || defined(TARGET_ARM)
    {
        uint64_t timer_val = 0;

        CPU_FOREACH(cpu) {
            if (cpu->kvm_run->exit_time > timer_val) {
                timer_val = cpu->kvm_run->exit_time;
            }
        }

        if (timer_val > 0) {
            CPU_FOREACH(cpu) {
                cpu->kvm_run->vmi_hide_time = timer_val;
            }
        }
    }
#endif

    CPU_FOREACH(cpu) {
        if (cpu->vmi_singlestep_enabled) {
            resume_cpu = cpu;
            break;
        }
    }

    if (resume_cpu != NULL) {
        cpu_resume(resume_cpu);
    }
    else {
        // Only enable BPs if we are not single stepping
        kvm_enable_phys_breakpoints();

        if (!runstate_is_running()){
            vm_start();
        }
        else{
            cpu_enable_ticks();
            resume_all_vcpus();
        }
    }

    return 1;
}

int vmi_wait_event(time_t secs){
    int r = 0;
    while(QSIMPLEQ_EMPTY(vmi_event_queue)) {
        r = qemu_mutex_timedwait_iothread(&vmi_event_cv, secs);
        if (r)
            break;
    }
    return r;
}

void vmi_stop_vm(void) {
    cpu_disable_ticks();
    pause_all_vcpus();

    kvm_disable_phys_breakpoints();
    dpy_mouse_out();
}

void vmi_wait_init(void){
    while(QSIMPLEQ_EMPTY(vmi_event_queue)) {
        qemu_mutex_wait_iothread(&vmi_event_cv);
    }
}

