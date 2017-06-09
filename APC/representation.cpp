#ifndef REPR_APC
#define REPR_APC

#include <vector>
#include "dataset.hpp"
#include <algorithm>

using namespace std;


class APC_Instance
{
public:
	vector<double> weights;

	// random initialization
	APC_Instance(int length, int seed){
		weights.resize(length);
		srand(seed);

		double maxElem = 1;
		// Generate random numbers for each element
		for (int i = 0; i < length; ++i){
		 	weights[i] = rand() % 100
		 	maxElem = max(maxElem,weights[i]);
		}

		// Normalize by dividing everything by the highest
		for (int i = 0; i < length; i++){
			weights[i] = weights[i]/maxElem;
		}
	}

	// From an existing vector
	APC_Instance(vector<double> init){
		weights = init;
	}

	~APC_Instance();
	

	vector<APC_Instance> genNeighbors(int numNeighbors, int seed);

	long double evaluate(){
		return 0;
	}

};






#endif