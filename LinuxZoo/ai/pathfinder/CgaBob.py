#-*- encoding: utf-8 -*-
# define the genetic algorithm class
import random
from CBobsMap import CBobsMap

class CgaBob:
    # the single genomes
    _SGenome = []
    # contain many genomes
    _vecGenomes = []
    # size of population
    _iPopSize = 0
    _dCrossoverRate = 0
    _dMutationRate = 0

    # how many bits per chromosome
    _iChromoLength = 0
    # how many bits per gene
    _iGeneLength = 500
    _ivecGenes = 1000
    _iFittestGenome = 0
    _dBestFitnessScore = 0
    _dTotalFitnessScore = 0
    _iGeneration = 0

    # Now, we should create some genomes, 
    # length -- one gene's length
    # amount -- the total number of genes
    def CreateGenes(self, length, amount):
        for i in range(amount):
            self._SGenome = []
            for j in range(length):
                self._SGenome.append(random.randint(0, 1))
            self._vecGenomes.insert(i, self._SGenome)

    # Now, we can also decode the binary genes
    def Decode(self, Genome):
        directions = []
        for i in range(0, len(Genome), 2):
            directions.append(int(str(Genome[i]) + str(Genome\
                    [i + 1]), 2))

        # OK, return directions
        return directions
    # store the best solution
    _fitness = 0
    # store the best steps
    _steps = 0
    # store the best genome
    _Genome = []
    _UserTrack  = []
    def UpdateScores(self, Genome, track, fitness, steps):
        # we get a solution
        if fitness == 0:
            if (self._steps == 0) or (self._steps > steps):
                self._steps = steps
                self._Genome = Genome
                self._UserTrack = track
        else:
            if (self._fitness == 0) or (self._fitness > fitness):
                self._fitness = fitness
                self._Genome = Genome 
                self._UserTrack = track

    # Now, let's create a auto find path machine
    def FindPath(self, mapBob):
        self.CreateGenes(self._iGeneLength, self._ivecGenes)
        # then we try every gene to find the solution
        for i in range(len(self._vecGenomes)):
            directions = self.Decode(self._vecGenomes[i])
            # Here, we get the fitness(if fitness equal to 0
            # then we get the solution, and the steps we use 
            # to get the solution, also we need a track of the
            # user track
            fitness, steps, track  = mapBob.TestRoute(directions)
            directions = []
            self.UpdateScores(self._vecGenomes[i], track, fitness, steps)
        # At last, put the best solution into user track
        mapBob._UserMap = self._UserTrack

