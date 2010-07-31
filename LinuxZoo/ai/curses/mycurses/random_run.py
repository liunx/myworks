#!/usr/bin/env python
#-*- encoding: utf-8 -*-

import curses
import time
import random


Pos = 3
Map = []
speed_y = 1
speed_x = 1
def draw_map():
    global Map
    Map = [
            [1, 1, 1, 1, 1, 1, 1, 1],
            [1, 0, 0, 0, 0, 0, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 1],
            [1, 1, 1, 1, 1, 1, 1, 1]
            ]
    for i in range(8):
        for j in range(8):
            if Map[i][j] == 1:
                stdscr.addch(i + Pos, j + Pos + 1, ord('#'))

def collision_check(y, x):
    chyx = stdscr.inch(y, x)
    if (chyx == -1):
        return False
    if (chr(chyx) == '#'):
        return True
    return False

def random_run(y, x):
    # We should get the border of screen
    max_y, max_x = stdscr.getmaxyx()
    rand_yx = random.randint(0, 3)
    # Get the random directions
    if rand_yx == 0:
        y -= 1
    elif rand_yx == 1:
        y += 1
    elif rand_yx == 2:
        x += 1
    elif rand_yx == 3:
        x -= 1

    # Check the borders
    if y < 0:
        y = 0
    elif y > max_y - 1:
        y = max_y - 1

    if x < 0:
        x = 0
    elif x > max_x - 1:
        x = max_x - 1

    # Check the barriers
    if collision_check(y + 1, x):
        y -= 1
    elif collision_check(y - 1, x):
        y += 1
    elif collision_check(y, x + 1):
        x -= 1
    elif collision_check(y, x - 1):
        x += 1

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
        y, x = random_run(y, x)
        ch = stdscr.getch()
        if ch == ord('q') or ch == ord('Q'):
            return

        stdscr.erase()
        # erase and refresh should have a proper interval
        curses.napms(100)
        stdscr.refresh()

curses.wrapper(main)



