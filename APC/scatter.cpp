/**
 *  Universidad Simon Bolivar
 *  CI-5652 - Diseño de Algoritmos II
 *  @author : Erick Silva
 *  @author : Stefani Castellanos 11-11394
 *
 *  Simulated annealing function.
 */

#ifndef SCATTER_SEARCH
#define SCATTER_SEARCH

#include <vector>
#include "dataset.hpp"
#include "representation.cpp"
#include <algorithm>
#include <math.h>
#include <random>
#include "seed.hpp"
#include <utility> //pairs

using namespace std;

/**
 * Function that executes simulatedAnnealing given an initial instance
 *  uses boltz temperature.
 *
 * @params initial      : Initial good Instance obtained via greedy.
 * @params trainingSet  : the training set to be used in the KNN function.
 * @params testSet      : the test set to be used in the KNN function.
 * @params maxIterations: max iterations to use.
 *
 * @return The APC_instance with the best score it can find.
 * 
 */
APC_Instance scatter(APC_Instance initial,
                            DataSet &trainingSet, 
                            DataSet &testSet,
                            int popSize = 8,
                            int maxIterations = 300,
                            int maxIterationsWithoutChange){

    vector<APC_Instance> referenceSet (popSize);

    // 2 * Binomial (popsize 2)  + popsize for the original ones.
    int combinationListSize = popSize*(popSize-1)  + popSize;

    vector< pair<double,APC_Instance> > combinationsList (combinationListSize);

    int length = initial.weights.size();

    referenceSet[0] = initial;
    for (int i = 1; i < popSize; i++){
        referenceSet[i] = APC_Instance(length);
    }  

    vector<APC_Instance> children;
    int combinationsListIndex = 0;
    bool changeInRefSet = 0;
    int noChange = 0;
    for (int it = 0 ; it < maxIterations ; it++){
        //generate the combinationsList
        combinationsListIndex = 0;
        for (int i = 0 ; i < popSize; i++){
            combinationsList[combinationsListIndex] = make_pair(popSize[i].evaluate(trainingSet,testSet),
                                                                popSize[i]);
            combinationsListIndex++;
        }

        for (int i = 0; i < popSize; i++){
            for (int j = i+1; j < popSize;j++){
                children = referenceSet[i].blendAlphaCrossover(referenceSet[j]);
                for (APC_Instance child: children){
                    combinationsList[combinationsListIndex] = make_pair(child.evaluate(trainingSet,testSet),
                                                    child);
                    combinationsListIndex++;
                }
            }
        }

        sort(combinationsList.begin(),combinationsList.end());
        //Check for changes in ref set
        changeInRefSet = 0;
        for (int i = 0 ; i < popSize; i++){
            if (referenceSet[i] != combinationsList[i].second){
                changeInRefSet = 1;
                referenceSet[i] = combinationsList[i].second;
            }
        }

        if (!changeInRefSet){
            noChange++;
        }
        else{
            noChange = 0;
        }

        if (noChange > maxIterationsWithoutChange){
            break;
        }
    }

    return referenceSet[0];
}



#endif