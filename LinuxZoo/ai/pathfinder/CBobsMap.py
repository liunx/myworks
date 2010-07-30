#-*- encoding: utf-8 -*-
# If this file just a definition file, so
# we do not need to add the "/usr/bin/env python"

# CBobsMap class
import curses

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
    _iMapWidth = 0
    _iMapHeight = 0

    # index into the array which is the start point
    _iStartX = 0
    _iStartY = 0
    # the finish point
    _iEndX = 0
    _iEndY = 0

    def draw_map(self):


