#-*- encoding: utf-8 -*-
# define the genetic algorithm class
import random

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
    _iGeneLength = 0
    _iFittestGenome = 0
    _dBestFitnessScore = 0
    _dTotalFitnessScore = 0
    _iGeneration = 0

    # Now, we should create some genomes 
    def CreateGenes(self, num):
        for i in range(num):
            self._SGenome.append(random.randint(0, 1))

    # Now, we can also decode the binary genes
    def Decode(self):
        directions = []
        for i in range(0, len(self._SGenome), 2):
            directions.append(int(str(self._SGenome[i]) + str(self._SGenome\
                    [i + 1]), 2))

        # OK, return directions
        return directions



