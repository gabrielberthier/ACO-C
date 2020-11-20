//
// Created by gnber on 19/11/2020.
//

#ifndef ACO_ALGORITHM_ACO_H
#define ACO_ALGORITHM_ACO_H
void ShowAnts(int **ants, int **dists, int antsLen, int numCities);

int generateRandomFromRange(int lower, int uppper);

void Display(int * trail, int numCities);

int Distance(int cityX, int cityY, int **dists);

int ** MakeGraphDistances(int numCities);

int EdgeInTrail(int cityX, int cityY, int *trail, int numCities);

void UpdatePheromones(double ** pheromones, int ** ants, int ** dists, int antsLen, int numCities, double rho, double Q);

double *MoveProbs(int k, int cityX, int * visited, double **pheromones, int **dists, int numCities);

double randomdouble();

int NextCity(int k, int cityX, int *visited, double **pheromones, int **dists, int numCities);

int * BuildTrail(int k, int start, double ** pheromones, int ** dists, int numCities);

void UpdateAnts(int ** ants, double ** pheromones, int ** dists, int antsLen,int numCities);

double ** InitPheromones(int numCities);

int *CopyTo(int * from, int *to, int max);

int * BestTrail(int **ants, int ** dists, int numAnts, int numCities);

double Length(int *trail, int **dists, int trailLen);

int IndexOfTarget(int *trail, int target, int size);

int * RandomTrail(int start, int numCities);

int ** InitAnts(int numAnts, int numCities);

double randfrom(double min, double max);

int randomInt();
#endif //ACO_ALGORITHM_ACO_H
