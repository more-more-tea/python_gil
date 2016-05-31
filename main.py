#!/bin/python

import threading
import logging
# import zmq.green as zmq
import zmq

import pub
import proc

logging.basicConfig( format='[%(levelname)s] %(message)s', level=logging.DEBUG )

SUB_ADDR = 'tcp://localhost:10000'

def setupMqAndReceive():
    '''Setup the message queue and receive messages.
    '''
    ctx  = zmq.Context()
    sock = ctx.socket( zmq.SUB )
    # add topics
    sock.setsockopt_string( zmq.SUBSCRIBE, 'Hello' )

    sock.connect( SUB_ADDR )

    while True:
        msg = sock.recv().decode( 'utf-8' )
        proc.onMsg( msg )

def callback( a, b ):
    print( '[callback]',  a, b )

def main():
    '''Entrance of the module.
    '''
    pub.start()
    proc.setCallback( callback )

    threading.Thread( target=setupMqAndReceive ).start()
    proc.start()

    '''
    A runnable version is

    pub.start()
    gevent.spawn( setupMqAndReceive ).join()

    However, the received messages are not processed.
    '''
