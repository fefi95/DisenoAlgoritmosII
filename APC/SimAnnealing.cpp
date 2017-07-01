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
                            int neighborsPerGen = 10,
                            double temperatureDecrease = 0.003){
    printf("attributes maxIterations = %d, initialTemperature %d,neighborsPerGen %d, temperatureDecrease = %f \n",
             maxIterations,initialTemperature,neighborsPerGen,temperatureDecrease);
    double BestValue = initial.evaluate(trainingSet,testSet);
    double currentValue = BestValue;
    APC_Instance Best = initial;
    APC_Instance current = initial;

    vector<APC_Instance> neighbors;
    bool change;

    double temperature = initialTemperature;
    double probAcceptance;
    double neighborValue;
    double delta = 0;
    double random = 0;

    for (int i = 0; i < maxIterations and temperature > 1; ++i){
        change = true;
        while(change){
            change = false;
            neighbors = current.genNeighbors(neighborsPerGen);
            for (APC_Instance neighbor : neighbors){
                neighborValue = neighbor.evaluate(trainingSet,testSet);

                // If it's a better solution, change the current best one
                if (neighborValue > currentValue){
                    //printf("Change from %f to %f, best is %f\n",currentValue,neighborValue, BestValue );
                    current = neighbor;
                    currentValue = neighborValue;
                    change = true;
                    if (neighborValue > BestValue){
                        Best = neighbor;
                        BestValue = neighborValue;
                    }
                }
                else{
                    // Otherwise, probabilities!.
                    // calculating the current temperature as t0/lg(it+1);
                    //calculating probability of acceptance (1/(1+exp(delta/temp)))
                    delta = currentValue - neighborValue;
                    probAcceptance = 1 / (1 + exp( 1+delta / temperature));
                    // roll a dice from 0 to 1, assumes as random seed is set.
                    random = (double)rand()/(double)RAND_MAX;

                    if (random <= probAcceptance){
                        current = neighbor;
                        currentValue = neighborValue;
                        change = true;
                        //printf("prob %f rolled %f Temp Change from %f to %f, best is %f\n",probAcceptance,random,currentValue,neighborValue, BestValue );
                    }
                }
            }
        }
        temperature = (double)initialTemperature/log(i+2);
        //printf("temperature %f\n",temperature);
    }

    return Best;
}



#endif