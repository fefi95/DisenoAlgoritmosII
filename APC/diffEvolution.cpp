/**
 *  Universidad Simon Bolivar
 *  CI-5652 - Diseño de Algoritmos II
 *  @author : Erick Silva
 *  @author : Stefani Castellanos 11-11394
 *
 *  Differential Evolution Function.
 */

#ifndef DIF_EVOLUTION
#define DIF_EVOLUTION

#include <vector>
#include "dataset.hpp"
#include "representation.cpp"
#include <algorithm>
#include <math.h>
#include <random>
#include "seed.hpp"

using namespace std;

/**
 * Function that executes simulatedAnnealing given an initial instance
 *  uses boltz temperature.
 *
 * @params lengthInstance   : the legnth of the instance used.
 * @params trainingSet  : the training set to be used in the KNN function.
 * @params testSet      : the test set to be used in the KNN function.
 * @params popSize  : population size.
 * @params CR   : probability of using the mutation over the original.
 * @params F    : multiplier for mutation.
 * @params maxIterations    : max iterations to use.
 * @params maxItersWithNoChange : max iterations to keep going without change.
 *
 * @return The APC_instance with the best score it can find.
 * 
 */
APC_Instance difEvolution(int lengthInstance,
                            DataSet &trainingSet, 
                            DataSet &testSet, 
                            int popSize = 100,
                            double CR = 0.5,
                            double F = 0.5,
                            int maxIterations = 300,
                            int maxItersWithNoChange = 2){


    //Assumes std::mt19937 rng(seed); has been set. 
    //Sets the random.
    uniform_int_distribution<int> gen(0, popSize-1);
    uniform_real_distribution<double> genDouble(0,1);

    // Creates the population.
    vector<APC_Instance> population(popSize);
    for (int i = 0 ; i < popSize; i++){
        population[i] = APC_Instance(lengthInstance);
    }

    APC_Instance mutation(lengthInstance);
    int r1,r2,r3; //randoms for mutation
    double rIndex;
    int iterWithNoChange = 0;
    bool change = 0;

    for (int i = 0; i < maxIterations; ++i){
        change = 0;

        for (int j = 0; j < popSize; ++j){
            r1 = gen(rng); // Assumes the variable rng has been set.
            r2 = gen(rng);
            r3 = gen(rng);
            //generate the values for the mutation.
            for (int k = 0; k < lengthInstance; ++k){
                rIndex = genDouble(rng);
                //If our random is less than CR, do the mutation
                // otherwise just keep the original.
                if (rIndex < CR){
                    mutation.weights[k] = population[r1].weights[k] + 
                        F*(population[r2].weights[k] - population[r3].weights[k]);
                }
                else{
                    mutation.weights[k] = population[j].weights[k];
                }
            }

            // Compare mutation with original
            if (mutation.evaluate(trainingSet,testSet) > population[j].evaluate(trainingSet,testSet)){
                population[j] = mutation;
                change = 1;
                iterWithNoChange = 0;
            }
        }
        if (!change){
            iterWithNoChange++;
        }
        // If we go a certain number of iterations without changing
        //  we can assume it's done
        if (iterWithNoChange >= maxItersWithNoChange){
            break;
        }
    }

    APC_Instance bestInstance;
    double bestValue = 0;
    double auxValue;
    // Let's look for the best instance
    for (int i = 0; i < popSize; i++){
        auxValue = population[i].evaluate(trainingSet,testSet);
        if (auxValue > bestValue){
            bestInstance = population[i];
            bestValue = auxValue;
        }
    }

    return bestInstance;
}



#endif