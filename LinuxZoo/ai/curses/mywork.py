#!/usr/bin/env python
#-*- encoding: utf-8 -*-

import curses
import time

def main(win):
    global stdscr
    stdscr = win

    if curses.has_colors():
        bg = curses.COLOR_BLACK
        curses.init_pair(1, curses.COLOR_BLUE, bg)
        curses.init_pair(2, curses.COLOR_CYAN, bg)
    curses.nl() # new line
    curses.noecho()
    stdscr.timeout(0)

    x = 0
    y = 0
    while True:
        stdscr.addch(x, y, ord('#'))
        ch = stdscr.getch()
        if ch == ord('q') or ch == ord('Q'):
            return
        elif ch == curses.KEY_UP:
            x += 1
        elif ch == curses.KEY_DOWN:
            x -= 1
        elif ch == curses.KEY_LEFT:
            y += 1
        elif ch == curses.KEY_RIGHT:
            y -= 1
        stdscr.erase()
        stdscr.refresh()

        curses.napms(50)

curses.wrapper(main)



