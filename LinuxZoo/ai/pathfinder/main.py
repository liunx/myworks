#!/usr/bin/env python
#-*- encoding: utf-8 -*-

from CBobsMap import CBobsMap
from CgaBob import CgaBob
import curses

def main(win):
    global stdscr
    stdscr = win
    # It's used to store the directions
    directions = []
    if curses.has_colors():
        bg = curses.COLOR_BLACK
        curses.init_pair(1, curses.COLOR_BLUE, bg)
        curses.init_pair(2, curses.COLOR_CYAN, bg)
    curses.nl()
    curses.noecho()
    stdscr.timeout(0)

    mymap = CBobsMap()
    myBob = CgaBob()

    myBob.CreateGenes(1000)

    directions = myBob.Decode()

    while True:
        ch = stdscr.getch()
        if ch == ord('q') or ch == ord('Q'):
            return
        #stdscr.erase()
        mymap.draw_map(stdscr) 
        mymap.TestRoute(directions)
        mymap.draw_track(stdscr)

        curses.napms(100)
        stdscr.refresh()

curses.wrapper(main)
