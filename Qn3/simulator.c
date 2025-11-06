// simulator.c
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>
#include <time.h>
#include <stdlib.h>

typedef struct {
    PyObject_HEAD
    double duration;    // total seconds to simulate (e.g., 30.0)
    double interval;    // seconds between readings (e.g., 2.0)
    double start_time;  // epoch at creation
    int steps_emitted;  // how many values emitted
    int max_steps;
} SimulatorObject;

/* Utility: get current time as double seconds since epoch */
static double now_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec + ts.tv_nsec/1e9;
}

/* Sleep for a fractional number of seconds while allowing other Python threads */
static void sleep_seconds(double s) {
    if (s <= 0.0) return;
    struct timespec req;
    req.tv_sec = (time_t) s;
    req.tv_nsec = (long) ((s - req.tv_sec) * 1e9);
    // Release GIL while sleeping
    Py_BEGIN_ALLOW_THREADS
        nanosleep(&req, NULL);
    Py_END_ALLOW_THREADS
}

/* Constructor: Simulator(duration, interval) */
static int Simulator_init(SimulatorObject *self, PyObject *args, PyObject *kwds) {
    double duration = 30.0;
    double interval = 2.0;
    static char *kwlist[] = {"duration", "interval", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|dd", kwlist, &duration, &interval))
        return -1;

    if (interval <= 0.0 || duration <= 0.0) {
        PyErr_SetString(PyExc_ValueError, "duration and interval must be positive");
        return -1;
    }

    self->duration = duration;
    self->interval = interval;
    self->start_time = now_seconds();
    self->steps_emitted = 0;
    self->max_steps = (int) (duration / interval) + 1; // +1 to include t=0 reading

    // seed only once using start_time
    srand((unsigned int)(self->start_time));
    return 0;
}

/* Dealloc */
static void Simulator_dealloc(SimulatorObject *self) {
    Py_TYPE(self)->tp_free((PyObject *) self);
}

/* iterator protocol: __iter__ returns self */
static PyObject *Simulator_iter(PyObject *self) {
    Py_INCREF(self);
    return self;
}

/* Helper to produce a simulated reading based on an index/time */
static void make_reading(int step_index, double timestamp, double *temp, double *hum, double *bat) {
    // deterministic-ish variation using step_index and rand
    double base_temp = 20.0 + 5.0 * (sin(step_index * 0.3) );   // 15..25-ish
    double noise_t = ((double)(rand() % 100) / 100.0 - 0.5) * 0.6;
    *temp = base_temp + noise_t;

    double base_h = 40.0 + 20.0 * (cos(step_index * 0.25)); // 20..60
    double noise_h = ((double)(rand() % 100) / 100.0 - 0.5) * 1.5;
    *hum = fmax(0.0, base_h + noise_h);

    // battery slowly decreasing with steps
    double drain_per_step = 0.5; // percent per step approx
    *bat = fmax(0.0, 100.0 - drain_per_step * step_index + ((double)(rand() % 100)/100.0 - 0.5) * 1.0);
}

/* iternext: produce next reading or raise StopIteration */
static PyObject *Simulator_iternext(SimulatorObject *self) {
    double elapsed = now_seconds() - self->start_time;
    int current_step = self->steps_emitted;

    if (elapsed >= self->duration && current_step > 0) {
        // finished; return StopIteration
        PyErr_SetNone(PyExc_StopIteration);
        return NULL;
    }

    if (current_step >= self->max_steps) {
        PyErr_SetNone(PyExc_StopIteration);
        return NULL;
    }

    // Calculate next reading timestamp: start_time + current_step * interval
    double target_timestamp = self->start_time + current_step * self->interval;

    // Sleep until target time (but not negative)
    double now = now_seconds();
    double to_sleep = target_timestamp - now;
    if (to_sleep > 0) {
        sleep_seconds(to_sleep);
    }
    // After sleeping, recompute timestamp to be precise
    double reading_time = now_seconds();

    double temp, hum, bat;
    make_reading(current_step, reading_time, &temp, &hum, &bat);

    // Build tuple (timestamp, temperature, humidity, battery)
    PyObject *tup = PyTuple_New(4);
    if (!tup) return NULL;
    PyTuple_SET_ITEM(tup, 0, PyFloat_FromDouble(reading_time));
    PyTuple_SET_ITEM(tup, 1, PyFloat_FromDouble(temp));
    PyTuple_SET_ITEM(tup, 2, PyFloat_FromDouble(hum));
    PyTuple_SET_ITEM(tup, 3, PyFloat_FromDouble(bat));

    self->steps_emitted += 1;
    return tup;
}

/* Methods / members */
static PyMemberDef Simulator_members[] = {
    {"duration", T_DOUBLE, offsetof(SimulatorObject, duration), 0, "duration (seconds)"},
    {"interval", T_DOUBLE, offsetof(SimulatorObject, interval), 0, "interval (seconds)"},
    {NULL}
};

static PyTypeObject SimulatorType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "simulator.Simulator",
    .tp_basicsize = sizeof(SimulatorObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor) Simulator_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Simulator(duration=30.0, interval=2.0) - yields (ts, temp, hum, bat)",
    .tp_members = Simulator_members,
    .tp_init = (initproc) Simulator_init,
    .tp_new = PyType_GenericNew,
    .tp_iter = Simulator_iter,
    .tp_iternext = (iternextfunc) Simulator_iternext,
};

/* Module-level method: convenience factory */
static PyObject *make_simulator(PyObject *self, PyObject *args, PyObject *kwds) {
    SimulatorObject *obj = (SimulatorObject *) PyObject_CallObject((PyObject *) &SimulatorType, args);
    if (!obj) return NULL;
    return (PyObject *) obj;
}

static PyMethodDef module_methods[] = {
    {"Simulator", (PyCFunction) make_simulator, METH_VARARGS | METH_KEYWORDS,
     "Create a Simulator(duration=30.0, interval=2.0)"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef simmodule = {
    PyModuleDef_HEAD_INIT,
    "simulator",
    "Lightweight IoT simulator providing an iterator yielding sensor readings.",
    -1,
    module_methods
};

PyMODINIT_FUNC PyInit_simulator(void) {
    PyObject *m;
    if (PyType_Ready(&SimulatorType) < 0)
        return NULL;

    m = PyModule_Create(&simmodule);
    if (!m) return NULL;

    Py_INCREF(&SimulatorType);
    if (PyModule_AddObject(m, "Simulator", (PyObject *) &SimulatorType) < 0) {
        Py_DECREF(&SimulatorType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}

