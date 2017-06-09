/**
 *  Universidad Simon Bolivar
 *  CI-5652 - Diseño de Algoritmos II
 *  @author : Erick Silva
 *  @author : Stefani Castellanos 11-11394
 *
 *  A Nearest Neighbor implementation.
 */


#ifndef NNeighbor1
#define NNeighbor1 

#include <vector>
#include "dataset.hpp"
#include <cmath>
#include <algorithm>

using namespace std;



/**
 * Function that calculates the euclidean distance between 2 vectors
 *  (taking them as an array)
 *
 * @params inst1     : Vector 1 of the dataset
 * @params inst2     : Vector 2 of the dataset
 * @params nFeatures : Size of vectors, not checked to save time
 *
 * @return : euclidean distance between the two vectors.
 */

double distance(vector<double> inst1, vector<double> inst2, int nFeatures) {
    double dist = 0;
    for (int i = 0; i < nFeatures; ++i) {
        dist = dist + pow(inst1[i] - inst2[i], 2.0);
    }
    dist = sqrt(dist);
    return dist;
}


// NAIVE IMPLEMENTATION, IF THIS BECOMES A PROBLEM,
// UPGRADE TO K-D TREE.
/**
 * Function that performs the nearest neighbor classifier
 *  and returns percentage of correct classifications. 
 *
 * @params trainingSet:  set used for training.
 * @params testSet   :   testing sets.
 *
 * @return : percentage of test set classified correctly.
 */
long double NN1(DataSet trainingSet, DataSet testSet){
    int nFeatures = trainingSet.nFeatures;
    int nTestInstances = testSet.nInstances;
    int nTrainInstances = trainingSet.nInstances;

    int correctResults = 0;
    double minDistance;
    double minDistancePos;
    double currentDistance;

    for (int i = 0; i < nTestInstances; ++i){        
        minDistance = INFINITY;
        minDistancePos = 0;
        for (int j = 0; j < nTrainInstances; ++j){
            currentDistance = distance(testSet.instances[i],
                                        trainingSet.instances[j]);
            if (currentDistance < minDistance){
                minDistance = currentDistance;
                minDistancePos = j;
            }
        }
        if (testSet.result[i] == trainingSet.result[minDistancePos]){
            correctResults++;
        }
    }
    
    double correctPercentage = (double)correctResults/(double)nTestInstances
    correctPercentage = 100*correctPercentage;

    return correctPercentage
}


#endif