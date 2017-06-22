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
 //
 // s∗ = local search(s0) ; /∗ Apply a given local search algorithm ∗/
 // Repeat
 // s′ = Perturb (s∗, search history) ; /∗ Perturb the obtained local optima ∗/
 // s′∗ = Local search (s′) ; /∗ Apply local search on the perturbed solution ∗/
 // s∗ = Accept (s∗, s′∗, search memory) ; /∗ Accepting criteria ∗/
 // Until Stopping criteria
 // Output: Best solution found.
APC_Instance iteratedLocalSearch(APC_Instance initial,
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


#endif // ILS_HPP
