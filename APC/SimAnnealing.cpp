/**
 *  Universidad Simon Bolivar
 *  CI-5652 - Diseño de Algoritmos II
 *  @author : Erick Silva
 *  @author : Stefani Castellanos 11-11394
 *
 *  Simulated annealing function.
 */

#ifndef SIM_ANNEALING
#define SIM_ANNEALING

#include <vector>
#include "dataset.hpp"
#include "representation.cpp"
#include <algorithm>
#include <math.h>
#include <stdlib.h> //random


using namespace std;

/**
 * Function that executes simulatedAnnealing given an initial instance
 *  uses boltz temperature.
 *
 * @params initial      : seed for random number generation.
 * @params trainingSet  : the training set to be used in the KNN function.
 * @params testSet      : the test set to be used in the KNN function.
 * @params maxIterations: max iterations to use.
 * @params initialTemperature: temperature to begin the annealing with.
 * @params neighborsPerGen: max neighbors to generate each time.
 *
 * @return The APC_instance with the best score it can find.
 * 
 */
APC_Instance simulatedAnnealing(APC_Instance initial, 
                            DataSet &trainingSet, 
                            DataSet &testSet, 
                            int maxIterations = 300,
                            int initialTemperature = 100,
                            int neighborsPerGen = 10){

    double currentBestValue = initial.evaluate(trainingSet,testSet);
    APC_Instance currentBest = initial;
    vector<APC_Instance> neighbors;
    bool change;

    double temperature = 0;
    double probAcceptance = 0;
    double neighborValue = 0;
    double delta = 0;
    double random = 0;
    for (int i = 0; i < maxIterations; ++i){
        change = false;
        neighbors = currentBest.genNeighbors(neighborsPerGen);
        for (APC_Instance neighbor : neighbors){
            neighborValue = neighbor.evaluate(trainingSet,testSet);

            // If it's a better solution, change the current best one
            if (neighborValue > currentBestValue){
                currentBest = neighbor;
                currentBestValue = neighborValue;
                change = 1;
            }
            else{
                // Otherwise, probabilities!.
                // calculating the current temperature as t0/lg(it+1);
                temperature = initialTemperature/log(i+1);
                //calculating probability of acceptance (1/(1+exp(delta/temp)))
                delta = currentBestValue - neighborValue;
                probAcceptance = 1 / (1 + exp( delta / temperature));
                // roll a dice from 0 to 1, assumes as random seed is set.
                random = rand()/RAND_MAX;

                if (random <= probAcceptance){
                    currentBest = neighbor;
                    currentBestValue = neighborValue;
                    change = 1;
                }
            }
        }

    }

    return currentBest;
}



#endif