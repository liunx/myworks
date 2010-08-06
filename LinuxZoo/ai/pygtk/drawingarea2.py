#!/usr/bin/env python
#-*- encoding: utf-8 -*-

import pygtk
pygtk.require('2.0')
import gtk

def main():
    window = gtk.Window(gtk.WINDOW_TOPLEVEL)
    window.set_name("Test Input")

    vbox = gtk.VBox(False, 0)
    window.add(vbox)
    vbox.show()

    window.connect("destroy", lambda w: gtk.main_quit())

    # Create the drawing area
    drawing_area = gtk.DrawingArea()
    drawing_area.set_size_request(200, 200)
    vbox.pack_start(drawing_area, True, True, 0)

    # now let's draw a point
    style = drawing_area.get_style()
    gc = style.fg_gc[gtk.STATE_NORMAL]
    drawing_area.show()
    window.show()
    drawing_area.window.draw_point(gc, 20, 30)


    gtk.main()

    return 0

if __name__ == "__main__":
    main()
