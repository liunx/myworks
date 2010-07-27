#!/usr/bin/env python
#-*- encoding: utf-8 -*-
import sys
import curses

Chess = [[0] * 8 for row in range(8)]

def main(win):
    global stdscr
    stdscr = win
    if curses.has_colors():
        bg = curses.COLOR_BLACK
        curses.init_pair(1, curses.COLOR_BLUE, bg)
        curses.init_pair(2, curses.COLOR_CYAN, bg)
    curses.nl()
    curses.noecho()

    for x in range(8):
        Chess[x][x] = 1
        for i in range(8):
            for j in range(8):
                #print Chess[i][j],
                stdscr.addch(i, j, ord('+'))
                curses.napms(50)
            #print
        stdscr.refresh()

if __name__ == '__main__':
    curses.wrapper(main)

