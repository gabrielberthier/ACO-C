//
// Created by gnber on 19/11/2020.
//

#include "ACO.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <float.h>



int randomInt(){

    int r = rand();
    return r;
}

int generateRandomFromRange(int lower, int upper){

    return  (rand() %
             (upper - lower + 1)) + lower;
}

int Distance(int cityX, int cityY, int **dists)
{
    return dists[cityX][cityY];
}

int arrayLength(int array[]){
    size_t length = sizeof(array) / sizeof(array[0]);
    return length;
}

int arrayLengthInt(int * array){
    int i = 0;
    while(array[i++]){

    }
    return i;
}

int ** InitAnts(int numAnts, int numCities)
{
    int ** ants =  (int **)malloc(numAnts * sizeof(int *));
    for (int k = 0; k <= numAnts - 1; k++)
    {
        int start = randomInt()%numCities;
        ants[k] = RandomTrail(start, numCities);
    }
    return ants;
}

int * RandomTrail(int start, int numCities)
{
    // helper for InitAnts
    int *trail = (int *)calloc(numCities, sizeof(int));

    // sequential
    for (int i = 0; i <= numCities - 1; i++)
    {
        trail[i] = i;
    }

    // Fisher-Yates shuffle
    for (int i = 0; i <= numCities - 1; i++)
    {
        int r = generateRandomFromRange(i, numCities-1);
        int tmp = trail[r];
        trail[r] = trail[i];
        trail[i] = tmp;
    }

    int idx = IndexOfTarget(trail, start, numCities);
    // put start at [0]
    int temp = trail[0];
    trail[0] = trail[idx];
    trail[idx] = temp;

    return trail;
}

int IndexOfTarget(int *trail, int target, int size)
{
    // helper for RandomTrail
    for (int i = 0; i <= size - 1; i++)
    {
        if (trail[i] == target)
        {
            return i;
        }
    }
    return -1;
}

double Length(int *trail, int **dists, int trailLen)
{
    // total length of a trail
    double result = 0.0;
    for (int i = 0; i <= trailLen - 2; i++)
    {
        result += Distance(trail[i], trail[i + 1], dists);
    }
    return result;
}

// --------------------------------------------------------------------------------------------

int * BestTrail(int **ants, int ** dists, int numAnts, int numCities)
{
    // best trail has shortest total length
    double bestLength = Length(ants[0], dists, numCities);
    int idxBestLength = 0;
    for (int k = 1; k <= numAnts - 1; k++)
    {
        double len = Length(ants[k], dists, numCities);
        if (len < bestLength)
        {
            bestLength = len;
            idxBestLength = k;
        }
    }

    //INSTANT VB NOTE: The local variable bestTrail was renamed since Visual Basic will not allow local variables with the same name as their enclosing function or property:
    int *bestTrail_Renamed = (int *) malloc(sizeof(int) * numCities);
    bestTrail_Renamed = CopyTo(ants[idxBestLength], bestTrail_Renamed, numCities);

    return bestTrail_Renamed;
}

int *CopyTo(int * from, int *to, int max){
    for(int i =0; i< max; i++){
        to[i] = from[i];
    }
    return to;
}

// --------------------------------------------------------------------------------------------

double ** InitPheromones(int numCities)
{
    double ** pheromones = (double **) malloc(sizeof(double *) * numCities);

    for (int i = 0; i <= numCities - 1; i++)
    {
        pheromones[i] = (double *) malloc(sizeof(double) * numCities);
    }
    for (int i = 0; i <= numCities - 1; i++)
    {
        for (int j = 0; j <= numCities - 1; j++)
        {
            pheromones[i][j] = 0.01;
            // otherwise first call to UpdateAnts -> BuildTrail -> NextNode -> MoveProbs => all 0.0 => throws
        }
    }

    return pheromones;
}

// --------------------------------------------------------------------------------------------

void UpdateAnts(int ** ants, double ** pheromones, int ** dists, int antsLen,int numCities)
{
    for (int k = 0; k <= antsLen - 1; k++)
    {
        int start = randomInt() % numCities;

        int *newTrail = BuildTrail(k, start, pheromones, dists, numCities);
        ants[k] = newTrail;
    }
}

int * BuildTrail(int k, int start, double ** pheromones, int ** dists, int numCities)
{
    int *trail = (int *) calloc(numCities, sizeof(int));
    int *visited = (int *) calloc(numCities, sizeof(int));;
    trail[0] = start;
    visited[start] = 1;
    for (int i = 0; i <= numCities - 2; i = i +1)
    {
        int cityX = trail[i];

        int next = NextCity(k, cityX, visited, pheromones, dists, numCities);

        trail[i + 1] = next;

        visited[next] = 1;
    }
    free(visited);
    return trail;
}

int NextCity(int k, int cityX, int *visited, double **pheromones, int **dists, int numCities)
{
    // for ant k (with visited[]), at nodeX, what is next node in trail?
    double *probs = MoveProbs(k, cityX, visited, pheromones, dists, numCities);

    double *cumul = (double *) calloc(numCities + 1, sizeof(double));
    for (int i = 0; i <= numCities - 2; i++)
    {
        cumul[i + 1] = cumul[i] + probs[i];
    }
    free(probs);
    cumul[numCities] = 1.0;

    double p = randomdouble();

    for (int i = 0; i <= numCities - 1; i++)
    {
//        printf("\n P(%lf) >= cumul[%d](%lf) && P(%lf) < cumul[%d](%lf)", p, i, cumul[i], p, i+1, cumul[i+1]);
        if (p >= cumul[i] && p < cumul[i + 1])
        {
            free(cumul);
            return i;
        }
    }
    printf("\nLeaving NextCity with an error");

    return -1;
}

double randomdouble(){
    return randfrom(0.01, 0.95);
}

double randfrom(double min, double max)
{
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

double *MoveProbs(int k, int cityX, int * visited, double **pheromones, int **dists, int numCities)
{

    // for ant k, located at nodeX, with visited[], return the prob of moving to each city
    double *taueta = (double *) calloc(numCities, sizeof(double));
    // inclues cityX and visited cities
    double sum = 0.0;
    // sum of all tauetas
    // i is the adjacent city
    for (int i = 0; i <= numCities - 1; i++)
    {
        if (i == cityX)
        {
            taueta[i] = 0.0;
            // prob of moving to self is 0
        }
        else if (visited[i])
        {
            taueta[i] = 0.0;
            // prob of moving to a visited city is 0
        }
        else
        {
            taueta[i] = pow(pheromones[cityX][i], 3.0) * pow((1.0 / Distance(cityX, i, dists)), 2.0);

            // could be huge when pheromone[][] is big
            if (taueta[i] < 0.001)
            {
                taueta[i] = 0.0001;
            }
            else if (taueta[i] > (DBL_MAX / (numCities * 100)))
            {
                taueta[i] = DBL_MAX / (numCities * 100);
            }
        }

        sum += taueta[i];
    }

    double * probs = (double *) calloc(numCities, sizeof(double));
    for (int i = 0; i <= numCities - 1; i++)
    {
        probs[i] = taueta[i] / sum;
        // big trouble if sum = 0.0
    }
    free(taueta);
    return probs;
}

// --------------------------------------------------------------------------------------------

void UpdatePheromones(double ** pheromones, int ** ants, int ** dists, int antsLen, int numCities, double rho, double Q)
{
    for (int i = 0; i <= numCities - 1; i++)
    {
        for (int j = i + 1; j <= numCities - 1; j++)
        {
            for (int k = 0; k <= antsLen - 1; k++)
            {
                double length = Length(ants[k], dists, numCities);
                // length of ant k trail
                double decrease = (1.0 - rho) * pheromones[i][j];
                double increase = 0.0;
                if (EdgeInTrail(i, j, ants[k], numCities))
                {
                    increase = (Q / length);
                }

                pheromones[i][j] = decrease + increase;

                if (pheromones[i][j] < 0.0001)
                {
                    pheromones[i][j] = 0.0001;
                }
                else if (pheromones[i][j] > 100000.0)
                {
                    pheromones[i][j] = 100000.0;
                }

                pheromones[j][i] = pheromones[i][j];
            }
        }
    }
}

int EdgeInTrail(int cityX, int cityY, int *trail, int numCities)
{
    // are cityX and cityY adjacent to each other in trail[]?
    int lastIndex = numCities - 1;
    int idx = IndexOfTarget(trail, cityX, numCities);

    if (idx == 0 && trail[1] == cityY)
    {
        return 1;
    }
    else if (idx == 0 && trail[lastIndex] == cityY)
    {
        return 1;
    }
    else if (idx == 0)
    {
        return 0;
    }
    else if (idx == lastIndex && trail[lastIndex - 1] == cityY)
    {
        return 1;
    }
    else if (idx == lastIndex && trail[0] == cityY)
    {
        return 1;
    }
    else if (idx == lastIndex)
    {
        return 0;
    }
    else if (trail[idx - 1] == cityY)
    {
        return 1;
    }
    else if (trail[idx + 1] == cityY)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


// --------------------------------------------------------------------------------------------

int ** MakeGraphDistances(int numCities)
{
    int **dists = (int **) calloc(numCities, sizeof(int *));

    for (int i = 0; i < numCities; i++)
    {
        *(dists + i) = (int *) calloc(numCities, sizeof(int));
    }

    for (int i = 0; i < numCities; i++)
    {
        for (int j = i + 1; j < numCities; j++)
        {
            int r = 1 + (rand() % 8);
            // [1,8]
            dists[i][j] = r;
            dists[j][i] = r;
        }
    }
    return dists;
}



// --------------------------------------------------------------------------------------------

void Display(int * trail, int numCities)
{
    for (int i = 0; i <= numCities - 1; i++)
    {
        printf("%d ", trail[i]);
        if (i > 0 && i % 20 == 0)
        {
            printf("");
        }
    }
    printf("");
}


void ShowAnts(int **ants, int **dists, int antsLen, int numCities)
{
    for (int i = 0; i <= antsLen - 1; i++)
    {
        printf("%d: [ ", i);

        for (int j = 0; j <= 3; j++)
        {
            printf("%d ", ants[i][j]);
        }

        printf(". . . ");

        for (int j = numCities - 4; j <= numCities - 1; j++)
        {
            printf("%d ", ants[i][j]);
        }

        printf("] len = ");
        double len = Length(ants[i], dists, numCities);
        printf("%lf\n", len);
        printf("");
    }
}