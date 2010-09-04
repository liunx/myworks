#!/usr/bin/env python
#-*- encoding: utf-8 -*-
# This program is used to detect server's state
# If the server crash, it can report it to web
# master

import sys
import socket
import os
import time

Host = 'localhost'
Port = 60002

class crashwatcher:
    def __init__(self):
        #print 'init the crashwatcher'
        pass

    # Bool con_server
    def con_server(self):
        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        except socket.error, msg:
            self.sock = None
            return False
        try:
            self.sock.connect((Host, Port))
        except socket.error, msg:
            self.sock.close()
            return False
        # if everything is ok
        return True

    # bool watch_server -- It will wait for server until crash
    # of course, i hope it wait forever
    def watch_server(self):
        while True:
            data = self.sock.recv(256)
            if not data:
                #print 'disconnect from remote server'
                self.sock.close()
                break
        self.SendMail()

    # restart the server, if it crashed
    def restart_server(self):
        #print 'Restart the server'
        os.system('/etc/init.d/lsServer start')

    # now, let's set up the watcher
    def do_work(self):
        self.do_daemon()
        i = 0
        # The big loop, never break out
        while True:
            if not self.con_server():
                time.sleep(3)
                i += 1
                if i > 10:
                    self.restart_server()
                    i = 0
                continue
            # Then we wait for server crash
            self.watch_server()

            # Oh, no
            #self.restart_server()
            #time.sleep(10)

    # We should run it in background
    def do_daemon(self):
        pid = os.fork()
        if pid > 0:
            sys.exit(1)
        os.chdir('/')

    # SendMail -- if crash happened, it should notify the
    # web master via email
    # We should fork a subprocess with a general user id 
    def SendMail(self):
        pid = os.fork()
        if pid > 0:
            return
        if pid == 0:
            os.setuid(1001)
            os.system('/home/enet/Shell/Mail/do_main.sh')

if __name__ == '__main__':
    cw = crashwatcher() 
    cw.do_work()


