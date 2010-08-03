#!/usr/bin/env python
#-*- encoding: utf-8 -*-

import pygtk
pygtk.require('2.0')
import gtk
import operator


class DrawingArea:
    def __init__(self):
        self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
        self.window.set_title("Drawing Area Example")
        self.window.connect("destroy", lambda w: gtk.main_quit())
        self.area = gtk.DrawingArea()
        self.area.set_size_request(400, 300)
        self.pangolayout = self.area.create_pango_layout("")
        self.sw = gtk.ScrolledWindow()
        self.sw.add_with_viewport(self.area)
        self.table = gtk.Table(2, 2)
        self.table.attach(self.sw, 1, 2, 1, 2)
        self.window.add(self.table)

    def show_area(self):
        self.area.show()
        self.sw.show()
        self.table.show()
        self.window.show()

    def draw_line(self, x, y):
        self.style = self.area.get_style()
        self.gc = self.style.fg_gc[gtk.STATE_NORMAL]
        self.area.window.draw_line(self.gc, x + 10, y + 10, x + 20, y + 30)
        self.pangolayout.set_text("Line")
        self.area.window.draw_layout(self.gc, x + 5, y + 50, self.pangolayout)
        return


def main():
    gtk.main()
    return 0

if __name__ == "__main__":
    draw_area = DrawingArea()
    draw_area.show_area()
    draw_area.draw_line(20, 30)

    main()
