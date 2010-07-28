#!/usr/bin/env python
#-*- encoding: utf-8 -*-

import curses
import time


Pos = 20
Map = []
def draw_map():
    global Map
    Map = [
            [1, 1, 1, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 1, 1, 1]
            ]
    for i in range(4):
        for j in range(4):
            if Map[i][j] == 1:
                stdscr.addch(i + Pos, j + Pos, ord('#'))

# Check whether we hit the barrier
def get_collision(y, x):
    global Map
    if y == Pos:
        if Map[y - Pos][x - Pos] == 1:
            stdscr.addch(10, 10, ord('*'))

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
        # We should get the border of screen
        max_y, max_x = stdscr.getmaxyx()
        draw_map()

        stdscr.addch(y, x, ord('#'))
        ch = stdscr.getch()
        if ch == ord('q') or ch == ord('Q'):
            return
        elif ch == curses.KEY_DOWN:
            y += 1
            if (y > max_y - 1):
                y = max_y - 1
            stdscr.erase()
        elif ch == curses.KEY_UP:
            y -= 1
            if (y < 0):
                y = 0
            stdscr.erase()
        elif ch == curses.KEY_RIGHT:
            x += 1
            if (x > max_x - 1):
                x = max_x - 1

            stdscr.erase()
        elif ch == curses.KEY_LEFT:
            x -= 1
            if (x < 0):
                x = 0
            stdscr.erase()

        y, x = get_collision(y, x)
        stdscr.refresh()

        curses.napms(10)

curses.wrapper(main)



