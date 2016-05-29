#include <unistd.h>

#include <iostream>
#include <pthread.h>

#include <Python.h>
#include <zmq.h>

#define BUF_SIZE 1024
#define PUB_ADDR "tcp://*:10000"

static void *pubMsg( void *args ) {
    /* initialize message queue and start to send message */
    void *pCtx  = NULL;
    void *pSock = NULL;

    char message[ BUF_SIZE ] = { 0 };

    if ( ( pCtx = zmq_ctx_new() ) == NULL ) {
        std::cerr << "Error creating ZMQ context." << std::endl;
    } else if ( ( pSock = zmq_socket( pCtx, ZMQ_PUB ) ) == NULL ) {
        zmq_ctx_destroy( pCtx );
        std::cerr << "Error creating ZMQ socket." << std::endl;
    } else if ( zmq_bind( pSock, PUB_ADDR ) < 0 ) {
        zmq_close( pSock );
        zmq_ctx_destroy( pCtx );
        std::cerr << "Error binding ZMQ socket." << std::endl;
    } else {
        int id = 0;
        while ( 1 ) {
            ++id;
            int iMsgLen = snprintf( message, BUF_SIZE, "Hello, %d", id );
            if ( zmq_send( pSock, message, iMsgLen, 0 ) < 0 ) {
                std::cerr << "Error sending messages with content " << id << std::endl;
            } else {
                /* rest 2 seconds */
                std::cerr << "[server] message with ID " << id << " sent" << std::endl;
                sleep( 2 );
            }
        }
    }

    return NULL;
}

static PyObject *start( PyObject *self, PyObject *args ) {
    pthread_t tid = 0;
    int result = pthread_create( &tid, NULL, pubMsg, NULL );

    return PyLong_FromLong( result );
}

static PyMethodDef pubMethods[] = {
    { "start", start, METH_VARARGS, "start sending message." },
    { NULL, NULL, 0, NULL }
};

static struct PyModuleDef pubModule = {
    PyModuleDef_HEAD_INIT,
    "pub",
    "Publish message",
    -1,
    pubMethods
};

PyMODINIT_FUNC PyInit_pub( void ) {
    return PyModule_Create( &pubModule );
}
