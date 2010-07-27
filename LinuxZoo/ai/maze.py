#!/usr/bin/env python
#-*- encoding: utf-8 -*-
import sys
import curses

Chess = [[0] * 4 for row in range(4)]

Chess = [
        [1, 2, 3, 4],
        [4, 3, 2, 1],
        [5, 6, 7, 8],
        [8, 7, 6, 5]
        ]

def main():

    for i in range(4):
        for j in range(4):
            print Chess[i][j],
        print

if __name__ == '__main__':
    main()
