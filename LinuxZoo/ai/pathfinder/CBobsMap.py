#-*- encoding: utf-8 -*-
# If this file just a definition file, so
# we do not need to add the "/usr/bin/env python"

# CBobsMap class
import curses
import random

class CBobsMap:
    # Here are the private data
    # the _map store the actural map
    _map = [
            [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
            [1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1],
            [8, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1],
            [1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1],
            [1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1],
            [1, 1, 0, 0, 1, 1, 1, 0 ,0 ,0 ,0, 0, 1, 0, 1],
            [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1],
            [1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5],
            [1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1],
            [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
            ]
    # this map is used to record the user path
    _UserMap = [[0 for col in range(15)] for row in range(10)]
    _iMapWidth = 15
    _iMapHeight = 10

    # index into the array which is the start point
    _iStartX = 14
    _iStartY = 7
    # the finish point -- exit 
    _iEndX = 0
    _iEndY = 2

    _fitness = 0

    # we need a param to get curses object and y,x cordinates
    def draw_map(self, stdscr):
        for i in range(10):
            for j in range(15):
                if (self._map[i][j] == 1):
                    stdscr.addch(i, j, ord('#'))
                elif (self._map[i][j] == 5):
                    stdscr.addch(i, j, ord('<'))
                elif (self._map[i][j] == 8):
                    stdscr.addch(i, j, ord('@'))

    # check whether we hit the wall, if hits, we just ignore the 
    # one instruction
    # the track is a list which contain the instructions
    def TestRoute(self, track):
        posY = self._iStartY
        posX = self._iStartX

        # before we use the _UserMap, we should clear it firstly
        self._UserMap = [[0 for col in range(15)] for row in range(10)]

        for i in range(len(track)):
            direction = track[i]

            if direction == 0:
                if (posY - 1 < 0) or (self._map[posY - 1]\
                        [posX] == 1):
                    continue
                else:
                    posY -= 1

            elif  direction == 1:
                if (posY + 1 >= self._iMapHeight) or (self._map\
                        [posY + 1][posX] == 1):
                    continue
                else:
                    posY += 1

            elif direction == 2:
                if (posX + 1 >= self._iMapWidth) or (self._map\
                        [posY][posX + 1] == 1):
                    continue
                else:
                    posX += 1

            elif direction == 3:
                if (posX - 1 < 0) or (self._map[posY][posX - 1] \
                        == 1):
                    continue
                else:
                    posX -= 1
            else:
                break

            # let's track the man moves
            self._UserMap[posY][posX] = 1
            # If we find the exit, then we just return
            a = float(abs(posY - self._iEndY))
            b = float(abs(posX - self._iEndX))
            self._fitness = 1 / (a + b + 1)
            if self._fitness == 1:
                # If we get a solution, the less steps the better
                return (self._fitness, i, self._UserMap)
        # At last, the more small of self._fitness, the better
        return (self._fitness, i, self._UserMap)

    # Show the track of solution
    def draw_track(self, stdscr):
        for i in range(self._iMapHeight):
            for j in range(self._iMapWidth):
                if self._UserMap[i][j] == 1:
                    stdscr.addch(i, j, ord('*'))




