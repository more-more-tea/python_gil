#include <iostream>
#include <pthread.h>

#include <Python.h>
#include <zmq.h>

#define BUF_SIZE 1024

static PyObject *callback = NULL;
static int msgAvailable   = 0;
static char buf[ BUF_SIZE ];

static PyObject *setCallback( PyObject *self, PyObject *args ) {
    if ( callback != NULL ) {
        Py_DECREF( callback );
    }

    PyObject *obj = NULL;
    if ( PyArg_ParseTuple( args, "O", &obj ) && PyCallable_Check( obj ) ) {
        Py_INCREF( obj );
        callback = obj;
    }

    return Py_None;
}

static PyObject *onMsg( PyObject *self, PyObject *args ) {
    const char *msg;

    if ( PyArg_ParseTuple( args, "s", &msg ) ) {
        std::cerr << "[processor] " << msg << std::endl;
        snprintf( buf, BUF_SIZE, "%s", msg );
        PyObject_CallFunction( callback, "ss", "[processor] Processed", buf );
        msgAvailable = 1;
    }

    return Py_None;
}

static void *procThread( void *args ) {
    while ( 1 ) {
        PyGILState_STATE gState = PyGILState_Ensure();

        if ( msgAvailable ) {
            std::cerr << "[processor thread] " << buf << std::endl;
            PyObject_CallFunction( callback, "ss", "[processor thread] Processed", buf );
            msgAvailable = 0;
        }

        PyGILState_Release( gState );
    }

    return NULL;
}

static PyObject *start( PyObject *self, PyObject *args ) {
    pthread_t tid;
    pthread_create( &tid, NULL, procThread, NULL );
    std::cerr << "All created" << std::endl;

    Py_BEGIN_ALLOW_THREADS
    pthread_join( tid, NULL );
    Py_END_ALLOW_THREADS
    std::cerr << "[WARNING] Code should not reach here." << std::endl;

    return Py_None;
}

static PyMethodDef procMethods[] = {
    { "start", start, METH_VARARGS, "start sending message." },
    { "setCallback", setCallback, METH_VARARGS, "Set a notifier." },
    { "onMsg", onMsg, METH_VARARGS, "request to process message." },
    { NULL, NULL, 0, NULL }
};

static struct PyModuleDef procModule = {
    PyModuleDef_HEAD_INIT,
    "proc",
    "Publish message",
    -1,
    procMethods
};

PyMODINIT_FUNC PyInit_proc( void ) {
    return PyModule_Create( &procModule );
}
