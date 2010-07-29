#!/usr/bin/env python
#-*- encoding: utf-8 -*-

import curses
import time


Pos = 10
Map = []
speed_y = 1
speed_x = 1
def draw_map():
    global Map
    Map = [
            [1, 1, 1, 1, 1, 1, 1, 1],
            [1, 0, 0, 0, 0, 0, 0, 0],
            [1, 0, 0, 0, 0, 0, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 1],
            [1, 0, 1, 1, 1, 1, 1, 1]
            ]
    for i in range(8):
        for j in range(8):
            if Map[i][j] == 1:
                stdscr.addch(i + Pos, j + Pos, ord('#'))

def collision_check(y, x):
    chyx = stdscr.inch(y, x)
    if (chr(chyx) == '#'):
        return True
    return False

def auto_run(y, x):
    global speed_y
    global speed_x
    # We should get the border of screen
    max_y, max_x = stdscr.getmaxyx()

    y += speed_y
    x += speed_x
    if (y >= max_y - 1) or (y <= 0):
        speed_y = -speed_y
    elif collision_check(y, x):
        speed_y = -speed_y
        y += speed_y

    if (x >= max_x - 1) or (x <= 0):
        speed_x = -speed_x
    elif collision_check(y, x):
        speed_x = -speed_x
        x += speed_x

    return (y, x)

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
        draw_map()
        stdscr.addch(y, x, ord('*'))
        y, x = auto_run(y, x)
        ch = stdscr.getch()
        if ch == ord('q') or ch == ord('Q'):
            return

        stdscr.erase()
        # erase and refresh should have a proper interval
        curses.napms(100)
        stdscr.refresh()

curses.wrapper(main)



