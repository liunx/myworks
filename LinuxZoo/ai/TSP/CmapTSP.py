#-*- encoding: utf-8 -*-

import pygtk
pygtk.require('2.0')
import gtk
import time
from math import *
import random

class CmapTSP:

    def __init__(self):
        self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
        self.window.connect("delete_event", self.delete_event)

        self.window.connect("destroy", self.destroy)

        # We should a container box to store the button and drawing area
        self.vbox = gtk.VBox(False, 0)
        self.window.add(self.vbox)
        self.vbox.show()

        # Create the drawing area
        self.drawing_area = gtk.DrawingArea()
        # define the width and height
        self.width = 400
        self.height = 400
        self.drawing_area.set_size_request(self.width, self.height)
        self.pangoplayout = self.drawing_area.create_pango_layout("")
        self.vbox.pack_start(self.drawing_area, True, True, 0)
        self.drawing_area.show()

        self.drawing_area.set_events(gtk.gdk.POINTER_MOTION_MASK 
                                     | gtk.gdk.POINTER_MOTION_HINT_MASK
                                     | gtk.gdk.BUTTON_PRESS_MASK)

        self.drawing_area.connect("expose_event", self.expose_event)
        self.drawing_area.connect("button_press_event", self.button_press_event)

        self.button = gtk.Button("Start")
        # When button is pressed, let's start our tour
        self.button.connect("clicked", self.hello, None)
        self.vbox.pack_start(self.button, False, False, 0)

        self.button.show()
        self.window.show()
        self.style = self.drawing_area.get_style()
        self.gc = self.style.fg_gc[gtk.STATE_NORMAL]

    def hello(self, widget, data = None):
        #print self.city_cords
        self.StartTour()


    def delete_event(self, widget, event, data = None):
        print 'delete event happened'
        return False

    def destroy(self, widget, data = None):
        print 'destroy signal occurred'
        gtk.main_quit()

    def expose_event(self, widget, event):
        self.style = self.drawing_area.get_style()
        self.gc = self.style.fg_gc[gtk.STATE_NORMAL]

        self.draw_cities()

    def button_press_event(self, widget, event):
        pass
        #self.draw_arcs(int(event.x), int(event.y))

    def draw_arcs(self, x, y):
        self.drawing_area.window.draw_arc(self.gc, False, x, y, 10, 
                                          10, 0, 360 * 64)
        #self.pangoplayout.set_text("arcs %d %d " % (x, y))
        #self.drawing_area.window.draw_layout(self.gc, x + 5, y + 5, self.pangoplayout)

    def draw_line(self, xs, ys, xe, ye):
        self.drawing_area.window.draw_line(self.gc, xs, ys, xe, ye)


    #--------------- Draw Cities -----------------------------------------
    #   we should draw cities we want tour to first
    #---------------------------------------------------------------------
    def draw_cities(self):
        self.NumCities = 18
        margin = 50
        # now, get the radius
        radius = 0
        if self.height < self.width:
            radius = (self.height / 2) - margin
        else:
            radius = (self.width / 2) - margin

        # now, get the center cordinates of the circle
        Cx, Cy = (self.width / 2, self.height / 2)
        SegmentSize = 2 * pi / self.NumCities
        angle = 0
        # we need a global list to store the cities' cordinates
        self.city_cords = []
        while angle < 2 * pi:
            city_x = int(radius * sin(angle) + Cx)
            city_y = int(radius * cos(angle) + Cy)
            self.draw_arcs(city_x, city_y)
            angle += SegmentSize
            self.city_cords.append([city_x, city_y])

        #print self.city_cords

    #------------------- BestPossibleFitness ------------------------------
    #   We use this to get the smallest distance, the shorter 
    #   the better.
    #----------------------------------------------------------------------
    def CalculateBestPossibleRoute(self):
        self.BestPossibleFitness = 0
        for city in range(len(self.city_cords)):
            self.BestPossibleFitness += self.CalculateA2B(self.city_cords\
                    [city], self.city_cords[city + 1])
        # at last, we should add the last and first distances
        self.BestPossibleFitness += self.CalculateA2B(self.city_cords\
                [len(self.city_cords) - 1], self.city_cords[0])


    #------------------- Calculate Distance -------------------------------
    #   calculate the distance between two cities
    #----------------------------------------------------------------------
    def CalculateA2B(self, cityA, cityB):
        xDist = cityA[0] - cityB[0]
        yDist = cityA[1] - cityB[1]

        return sqrt(xDist * xDist + yDist * yDist)

    #------------------- New Population -------------------------------------
    #   At first, we should create a random tours to find the best fit one
    #------------------------------------------------------------------------
    def NewPopulations(self):
        # We can try to generate one genomes
        # we need to generate a lot of genes to get the most fit one
        self.amount_genes = 1000
        self.total_genes = [0] * self.amount_genes 
        for amount in range(self.amount_genes):
            self.genome = []
            for i in range(self.NumCities):
                num = random.randint(0, self.NumCities - 1)
                while self.CheckExist(num, self.genome):
                    num = random.randint(0, self.NumCities - 1)
                # Then store the number into list
                self.genome.append(num)

            #print self.genome 
            self.total_genes[amount] = self.genome
        #print self.total_genes

    # Check a random number whether is exist
    def CheckExist(self, num, genome):
        for i in range(len(genome)):
            if num == genome[i]:
                return True
        return False

    #-------------------- Draw Tour ----------------------------------------
    #   Finaly, we should show the best fit route on the wall
    #-----------------------------------------------------------------------
    def DrawTour(self, genome):
        for i in range(len(genome) - 1):
            self.draw_line(self.city_cords[genome[i]][0], self.city_cords\
                    [genome[i]][1], self.city_cords[genome[i + 1]][0], self.\
                    city_cords[genome[i + 1]][1])
            self.draw_line(self.city_cords[genome[len(genome) - 1]][0], self.\
                    city_cords[genome[len(genome) - 1]][1], self.city_cords\
                    [genome[0]][0], self.city_cords[genome[0]][1])


    #------------------- Start Tour ----------------------------------------
    #   Now, It's time for the saleman to have tour, after many tries,
    #   he will find a best fit one route
    #-----------------------------------------------------------------------
    def StartTour(self):
        # First, generate a new population
        self.NewPopulations()

        self.DrawTour(self.total_genes[0])




    def main(self):
        gtk.main()
        return 0


