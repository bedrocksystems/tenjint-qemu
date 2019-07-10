#include "qemu/osdep.h"

#include <Python.h>

#include "qemu/thread.h"
#include "hw/boards.h"

#include "vmi.h"

static QemuThread *python_thread = NULL;

static void* vmi_python_thread_func(void *arg){
    Py_Initialize();
    PyRun_SimpleString("print(\"Hello World\")");
    Py_Finalize();
    return NULL;
}

void vmi_init(MachineState *ms) {
    if(!ms->vmi){
        return;
    }
    printf("VMI init\n");

    python_thread = g_malloc0(sizeof(QemuThread));
    qemu_thread_create(python_thread, "python", vmi_python_thread_func,
                       NULL, 0);

    ms->vmi_initialized = true;
}

void vmi_setup_post(MachineState *ms, AccelState *accel){
    if(!ms->vmi){
        return;
    }
    printf("VMI setup post\n");
}

void vmi_uninit(MachineState *ms, AccelState *accel){
    if(!ms->vmi){
        return;
    }
    printf("VMI uninit\n");

    qemu_thread_join(python_thread);
    g_free(python_thread);
    python_thread = NULL;
}