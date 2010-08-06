#!/usr/bin/env python
#-*- encoding: utf-8 -*-

import pygtk
pygtk.require('2.0')
import gtk

class HelloWorld:

    def __init__(self):
        self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
        self.window.connect("delete_event", self.delete_event)

        self.window.connect("destroy", self.destroy)

        # We should a container box to store the button and drawing area
        self.vbox = gtk.VBox(False, 0)
        self.window.add(self.vbox)
        self.vbox.show()

        # Create the drawing area
        self.drawing_area = gtk.DrawingArea()
        self.drawing_area.set_size_request(400, 400)
        self.pangoplayout = self.drawing_area.create_pango_layout("")
        self.vbox.pack_start(self.drawing_area, True, True, 0)
        self.drawing_area.show()

        self.drawing_area.set_events(gtk.gdk.POINTER_MOTION_MASK 
                                     | gtk.gdk.POINTER_MOTION_HINT_MASK
                                     | gtk.gdk.BUTTON_PRESS_MASK)

        self.drawing_area.connect("motion_notify_event", self.motion_notify_event)
        self.drawing_area.connect("button_press_event", self.button_press_event)

        self.button = gtk.Button("Hello World")
        self.button.connect("clicked", self.hello, None)
        self.vbox.pack_start(self.button, False, False, 0)

        self.button.show()
        self.window.show()
        self.style = self.drawing_area.get_style()
        self.gc = self.style.fg_gc[gtk.STATE_NORMAL]


    def hello(self, widget, data = None):
        for i in range(10):
            for j in range(10):
                self.draw_arcs(i * 20, j * 20)
        print 'Hello world'

    def delete_event(self, widget, event, data = None):
        print 'delete event happened'
        return False

    def destroy(self, widget, data = None):
        print 'destroy signal occurred'
        gtk.main_quit()

    def motion_notify_event(self, widget, event):
        self.style = self.drawing_area.get_style()
        self.gc = self.style.fg_gc[gtk.STATE_NORMAL]

    def button_press_event(self, widget, event):
        self.draw_arcs(int(event.x), int(event.y))

    def draw_arcs(self, x, y):
        self.drawing_area.window.draw_arc(self.gc, False, x, y, 10, 
                                          10, 0, 360 * 64)
        self.pangoplayout.set_text("arcs %d %d " % (x, y))
        self.drawing_area.window.draw_layout(self.gc, x + 5, y + 5, self.pangoplayout)

    def draw_line(self, xs, ys, xe, ye):
        self.drawing_area.window.draw_line(self.gc, xs, ys, xe, ye)

    def main(self):
        gtk.main()
        return 0

if __name__ == "__main__":
    helloworld = HelloWorld()
    helloworld.main()

