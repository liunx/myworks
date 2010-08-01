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
    _iPopSize = 1400
    _dCrossoverRate = 0.7
    _dMutationRate = 0

    # how many bits per chromosome
    _iChromoLength = 0
    # how many bits per gene
    _iGeneLength = 500
    _ivecGenes = 100
    _iFittestGenome = 0
    _dBestFitnessScore = 0
    _dTotalFitnessScore = 0
    _iGeneration = 0

    # Store the fitnesses
    _fitnesslist = []

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
        if fitness == 1:
            if (self._steps == 0) or (self._steps > steps):
                self._steps = steps
                self._Genome = Genome
                self._UserTrack = track
        else:
            if (self._fitness == 0) or (self._fitness < fitness):
                self._fitness = fitness
                self._Genome = Genome 
                self._UserTrack = track

                self._dTotalFitnessScore += fitness

    # crossover two genes and create two new genes
    def Crossover(self, Dad, Mum):
        if (random.random() > self._dCrossoverRate) or (Dad == Mum):
            return (Dad, Mum)

        cp = random.randint(0, self._iGeneLength - 1)
        tmp = Dad[cp:]
        Dad[cp:] = Mum[cp:]
        Mum[cp:] = tmp

        return (Dad, Mum)

    # ------------------ RouletteWheelSelection ---------------------------
    #   selects a member of the population by using roulette wheel 
    # ---------------------------------------------------------------------
    def RouletteWheelSelection(self):
        fSlice = random.random() * self._dTotalFitnessScore
        cfTotal = 0.0
        SelectedGenome = 0
        for i in range(len(self._vecGenomes)):
            cfTotal += self._fitnesslist[i]
            if cfTotal > fSlice:
                SelectedGenome = i
                break
        return self._vecGenomes[SelectedGenome]

    # --------------------- Mutate -----------------------------------------
    #   iterates through each genome flipping the bits acording to the 
    #   mutation rate
    # ----------------------------------------------------------------------
    def Mutate(self, Genome):
        for i in range(self._iGeneLength):
            if (random.random() < self._dMutationRate):
                Genome[i] = int(not Genome[i])
        return Genome


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
            self._fitnesslist.insert(i, fitness)
        # At last, put the best solution into user track
        mapBob._UserMap = self._UserTrack

    # ---------------------- Epoch ----------------------------------------
    #   This is the workhorse of the GA. It first updates the fitness
    #   scores of the population then creates a new polulation of genomes
    #   using the Selection, Crossover and Mutation operations we have 
    #   discussed
    # ---------------------------------------------------------------------
    def Epoch(self, mapBob):
        self.FindPath(mapBob)

        newGenome = []

        for i in range(0, self._iPopSize, 2):
            # select 2 parents
            mum = self.RouletteWheelSelection()
            dad = self.RouletteWheelSelection()

            # operator - crossover
            baby1, baby2 = self.Crossover(dad, mum)

            # operator - mutate
            baby1 = self.Mutate(baby1)
            baby2 = self.Mutate(baby2)

            newGenome.insert(i, baby1)
            newGenome.insert(i + 1, baby2)
        # at last, update _vecGenomes 
        self._vecGenomes = newGenome

        self._iGeneration += 1
        if self._iGeneration < 10:
            self.Epoch(mapBob)

        # after a evolution, let's try find path again
        self.FindPath(mapBob)

