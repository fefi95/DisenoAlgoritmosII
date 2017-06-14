/**
 *  Universidad Simon Bolivar
 *  CI-5652 - Diseño de Algoritmos II
 *  @author : Erick Silva
 *  @author : Stefani Castellanos 11-11394
 *
 *  A local search function.
 */

#ifndef LOCAL_SEARCH
#define LOCAL_SEARCH

#include <vector>
#include "dataset.hpp"
#include "representation.cpp"
#include <algorithm>

using namespace std;

/**
 * Function that executes localSearch given an initial instance
 *  it tries to find a local optimum through it's neighbors
 *
 * @params initial      : seed for random number generation.
 * @params trainingSet  : the training set to be used in the KNN function.
 * @params testSet      : the test set to be used in the KNN function.
 * @params maxIterations: max iterations to use.
 * @params neighborsPerGen: max neighbors to generate each time.
 *
 * @return The APC_instance with the best score it can find.
 * 
 */
APC_Instance localSearch(APC_Instance initial, 
                            DataSet trainingSet, 
                            DataSet testSet, 
                            int maxIterations = 300,
                            int neighborsPerGen = 1){

    double currentBestValue = initial.evaluate(trainingSet,testSet);
    APC_Instance currentBest = initial;
    vector<APC_Instance> neighbors;
    bool change;

    double neighborValue = 0;

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
        }

        // If we don't find any change, we assume a local optimum.
        // Note: as our neighbors are random, this is not always true.
        if (!change){
            break;
        }
    }

    return currentBest;
}



#endif