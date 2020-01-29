/*
 * QEMU KVM VMI support
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

#include <Python.h>

#include "sysemu/cpus.h"
#include "sysemu/vmi_event.h"
#include "sysemu/vmi_api.h"
#include "sysemu/vmi.h"
#include "qemu/thread.h"
#include "qemu/main-loop.h"
#include "hw/boards.h"

#include "vmi.h"

#define PY_STRING_SZ 2048

static bool _vmi_initialized = false;
static QemuThread *python_thread = NULL;

bool is_vmi_thread(void) {
    if (python_thread == NULL)
        return false;
    return qemu_thread_is_self(python_thread);
}

static void* vmi_python_thread_func(void *arg){
    char *vmi_configs = (char*) arg;
    char py_string[PY_STRING_SZ] = {0};

    if (vmi_configs) {
        snprintf(py_string, PY_STRING_SZ,
                 "import tenjint\ntenjint.tenjint.run(\"%s\")\n", vmi_configs);
    }
    else {
        strncpy(py_string, "import tenjint\ntenjint.tenjint.run()\n",
                PY_STRING_SZ-1);
    }
    Py_Initialize();
    PyRun_SimpleString(py_string);
    Py_Finalize();
    return NULL;
}

int vmi_init(MachineState *ms) {
    int rv = 0;

    if(!ms->vmi){
        return rv;
    }

    rv = vmi_event_init(ms);
    if (rv < 0)
        return rv;

    python_thread = g_malloc0(sizeof(QemuThread));
    qemu_thread_create(python_thread, "python", vmi_python_thread_func,
                       ms->vmi_configs, 0);

    _vmi_initialized = true;

    return rv;
}

void vmi_setup_post(MachineState *ms, AccelState *accel){
    struct vmi_event *event = NULL;

    if(!_vmi_initialized){
        return;
    }

    event = g_malloc0(sizeof(struct vmi_event));
    event->type = VMI_EVENT_VM_READY;
    vmi_put_event(event);
}

void vmi_uninit(MachineState *ms, AccelState *accel){
    struct vmi_event *event = NULL;

    if(!ms->vmi){
        return;
    }

    _vmi_initialized = false;

    event = g_malloc0(sizeof(struct vmi_event));
    event->type = VMI_EVENT_VM_SHUTDOWN;
    vmi_put_event(event);

    qemu_mutex_unlock_iothread();
    qemu_thread_join(python_thread);
    qemu_mutex_lock_iothread();
    g_free(python_thread);
    python_thread = NULL;

    vmi_event_uninit(ms, accel);
}

bool vmi_initialized(void) {
    return _vmi_initialized;
}