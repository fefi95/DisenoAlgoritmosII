/**
 *  Universidad Simon Bolivar
 *  CI-5652 - Diseño de Algoritmos II
 *  @author : Erick Silva
 *  @author : Stefani Castellanos 11-11394
 *
 *  A Representation for an APC_instance
 */

#ifndef REPR_APC
#define REPR_APC

#include <vector>
#include "dataset.hpp"
#include "NearestNeighbor.cpp"
#include <algorithm>
#include <stdlib.h> //random numbers.

using namespace std;

// Function that returns a random double
// TODO: Make proper documentation.
double dRand(double minVal, double maxVal){
    double aux = (double)rand() / RAND_MAX;
    return minVal + aux * (maxVal - minVal);
}


class APC_Instance{
public:
	vector<double> weights;

	// random initialization
	APC_Instance(int length){
		weights.resize(length);
		//srand(seed); We assume it is already initialized

		// Generate random numbers for each element
		for (int i = 0; i < length; ++i){
		 	weights[i] = dRand(0,1);
		}

	}

	// From an existing vector
	APC_Instance(vector<double> init){
		weights = init;
	}


	APC_Instance genNeighbor(){
		vector<double> neighbor;
		int nFeatures = weights.size();
		int posToChange;
		double delta;

		// initialize the neighbor with previous value
		neighbor.assign(weights.begin(), weights.end());

		// Make Changes
		posToChange = rand() % nFeatures;
		delta = dRand(-1,1);
		neighbor[posToChange] += delta;

		// if the position ends up being less than 0, we set it to 0
		if (neighbor[posToChange] < 0) neighbor[posToChange] = 0;

		// if it ends up greater than 1, we normalize the vector
		// 	by dividing by it's new highest value.
		if (neighbor[posToChange] > 1){
			for (int i = 0 ; i < nFeatures; i++){
				neighbor[i] = neighbor[i]/neighbor[posToChange];
			}
		}

		return APC_Instance(neighbor);

	}

	vector<APC_Instance> genNeighbors(int numNeighbors){
		vector<APC_Instance> neighbors;
		vector<double> auxVect;
		int nFeatures = weights.size();
		int posToChange;
		double delta;
		for (int i = 0; i < numNeighbors; i++){
			auxVect.assign(weights.begin(), weights.end());
			posToChange = rand() % nFeatures;
			delta = dRand(-1,1);
			auxVect[posToChange] += delta;

			if (auxVect[posToChange] < 0) auxVect[posToChange] = 0;

			// We normalize if it ends up being greater than 1
			if (auxVect[posToChange] > 1){
				for (int i = 0 ; i < nFeatures; i++){
					auxVect[i] = auxVect[i]/auxVect[posToChange];
				}
			}

			neighbors.push_back(APC_Instance(auxVect));
		}

		return neighbors;
	}

  APC_Instance perturbSolution(){
    vector<APC_Instance> neighbor = this -> genNeighbors(1);
    neighbor = neighbor[0].genNeighbors(1);
    APC_Instance perturbation = neighbor[0];
    return perturbation;
  }

	double evaluate(DataSet trainingSet, DataSet testSet){
		return NN1(trainingSet, testSet, weights);
	}


};



#endif
