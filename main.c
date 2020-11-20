#include <stdlib.h>
#include <time.h>
#include "ACO.h"
#include <stdio.h>

void showGraph(int **dists, int numCities){
    printf("\nShowing graphs\n");

    for (int i = 0; i < numCities; ++i) {
        for (int j = 0; j < numCities; ++j) {
            printf("[%d][%d] = %d\t", i,j, dists[i][j]);
        }
    }
}

void showPheromones(double **pheromones, int numCities){
    for (int i = 0; i < numCities; ++i) {
        for (int j = 0; j < numCities; ++j) {
            printf("[%d][%d] = %lf\t", i,j, pheromones[i][j]);
        }
    }
}

int main()
{
    srand(time(NULL));
    // influence of pheromone on direction
    int alpha = 3;
// influence of adjacent node distance
    int beta = 2;

// pheromone decrease factor
    double rho = 0.01;
// pheromone increase factor
    double Q = 2.0;

    printf("\nBegin Ant Colony Optimization demo\n");

    int numCities = 60;
    int numAnts = 10;
    int maxTime = 3000;

    printf("Number of cities in problem = %d\n", numCities);

    printf("\nNumber ants = %d\n", numAnts);
    printf("Maximum time = %d\n", maxTime);

    printf("\nAlpha (pheromone influence) = ", alpha);
    printf("Beta (local node influence) = %d\n", beta);
    printf("Rho (pheromone evaporation coefficient) = %.2f\n", rho);
    printf("Q (pheromone deposit factor) = %.2f\n", Q);

    printf("\nInitialing dummy graph distances");
    int ** dists = MakeGraphDistances(numCities);

    printf("\nInitialing ants to random trails\n");
    int ** ants = InitAnts(numAnts, numCities);
    // initialize ants to random trails
    ShowAnts(ants, dists, numAnts, numCities);

    int * bestTrail = BestTrail(ants, dists, numAnts, numCities);
    // determine the best initial trail
    double bestLength = Length(bestTrail, dists, numCities);
    // the length of the best trail

    printf("\nBest initial trail length: %.2f\n", bestLength);
    Display(bestTrail, numCities);

    printf("\nInitializing pheromones on trails");
    double ** pheromones = InitPheromones(numCities);

    showPheromones(pheromones, numCities);

    int time = 0;
    printf("\nEntering UpdateAnts - UpdatePheromones loop\n");
    while (time < maxTime)
    {
        UpdateAnts(ants, pheromones, dists, numAnts, numCities);
        UpdatePheromones(pheromones, ants, dists, numAnts, numCities, rho, Q);

        int  *currBestTrail = BestTrail(ants, dists, numAnts, numCities);
        double currBestLength = Length(currBestTrail, dists, numCities);
        if (currBestLength < bestLength)
        {
            bestLength = currBestLength;
            bestTrail = currBestTrail;
            printf("New best length of %.2f found at time %d", bestLength, time);
        } else
            free(currBestTrail);
        time += 1;
    }

    printf("\nTime complete");
    showPheromones(pheromones, numCities);
    printf("\nBest trail found:");
    Display(bestTrail, numCities);
    printf("\nLength of best trail found: %.2f", bestLength);

    printf("\nEnd Ant Colony Optimization demo\n");
    getchar();

    return 0;
}
// Main

// --------------------------------------------------------------------------------------------

