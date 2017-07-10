/**
 *  Universidad Simon Bolivar
 *  CI-5652 - Diseño de Algoritmos II
 *  @author : Erick Silva
 *  @author : Stefani Castellanos 11-11394
 *
 *  Functions for reading the dataset
 */


#ifndef ILS_HPP
#define ILS_HPP

#include <string>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <random>
#include "local_search.cpp"
#include "representation.cpp"
using namespace std;

/**
 * Function that executes iterated local Search given an initial instance
 *  it tries to find a local optimum through it's neighbors.
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
APC_Instance ILS_maxIter(APC_Instance initial,
                                 DataSet trainingSet,
                                 DataSet testSet,
                                 int maxIterations = 300,
                                 int neighborsPerGen = 1){

    APC_Instance solution = localSearch(initial,
                                        trainingSet,
                                        testSet,
                                        maxIterations,
                                        neighborsPerGen);

    for (int i = 0; i < maxIterations; ++i){
        solution.perturbSolution();
        solution = localSearch(initial,
                               trainingSet,
                               testSet,
                               maxIterations,
                               neighborsPerGen);
    }

    return solution;
}
/**
 * Function that executes iterated local Search given an initial instance
 * it tries to find a local optimum through it's neighbors until the
 * solution evaluate to the same value after iterToStop iterations.
 *
 * @params initial      : seed for random number generation.
 * @params trainingSet  : the training set to be used in the KNN function.
 * @params testSet      : the test set to be used in the KNN function.
 * @params maxIterations: max iterations to use.
 * @params neighborsPerGen: max neighbors to generate each time.
 * @params iterToStop: amount of iterations with the same value befoore stoping
 *
 * @return The APC_instance with the best score it can find.
 *
 */
APC_Instance ILS_convergence(APC_Instance initial,
                             DataSet &trainingSet,
                             DataSet &testSet,
                             int maxIterations = 300,
                             int neighborsPerGen = 1,
                             int maxIterationsWithoutChange = 10){

    APC_Instance solution = localSearch(initial,
                                        trainingSet,
                                        testSet,
                                        maxIterations,
                                        neighborsPerGen);

    APC_Instance bestSol = solution;
    int totallIter = 0;
    int countIter = 0;
    double oldValue = solution.evaluate(trainingSet, testSet);
    double bestValue = solution.evaluate(trainingSet, testSet);
    double newValue;
    while (countIter != maxIterationsWithoutChange) {
        solution = bestSol.perturbSolution(countIter+2);
        solution = localSearch(initial,
                               trainingSet,
                               testSet,
                               maxIterations,
                               neighborsPerGen);    
        newValue = solution.evaluate(trainingSet, testSet);

        // Keep current best solution
        if (bestValue < newValue ) {
            bestSol = solution;
            bestValue = newValue;
            countIter = 0;
        }
        else {
            ++countIter;
        }

        oldValue = newValue;
        ++totallIter;
    }

    return bestSol;
}

#endif // ILS_HPP
