/**
 *  Universidad Simon Bolivar
 *  CI-5652 - Diseño de Algoritmos II
 *  @author : Erick Silva
 *  @author : Stefani Castellanos 11-11394
 *
 *  Greedy relief algorithm
 */


#ifndef RELIEF_HPP_
#define RELIEF_HPP_

#include <iostream>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <vector>
using namespace std;

/**
 * Function that calculates the euclidean distance between 2 instances
 *
 * @params inst1     : Instance 1 of the dataset
 * @params inst2     : Instance 2 of the dataset
 * @params nFeatures : Number of features of the dataset
 *
 * @return : euclidean distance between to instances of a dataset
 */

double distance(vector<double> inst1, vector<double> inst2, int nFeatures) {
    double dist = 0;
    for (int i = 0; i < nFeatures; ++i) {
        dist = dist + pow(inst1[i] - inst2[i], 2.0);
    }
    dist = sqrt(dist);
    return dist;
}

/**
 * Function calculates feature weights for a given dataset using the relief
 * algorithm
 *
 * @params trainingSet : Dataset partition for trainning
 *
 * @return : feature weights vector for the training set
 */

std:: vector<double> relief(std::pair < vector <vector <string> >, vector <string> > trainingSet) {
    int nFeatures = 0;
    int nInstances = 0;

    std:: vector< vector <double> > dsFeatures;

    for (std::vector< vector <string> >::iterator inst = trainingSet.first.begin(); inst != trainingSet.first.end(); ++inst) {
        std:: vector<double> fVector;
        nFeatures = 0;
        for (std::vector <string>::iterator value = inst -> begin(); value != inst -> end(); ++value) {
            ++nFeatures;
            fVector.push_back(stod(*value));
        }
        dsFeatures.push_back(fVector);
        ++nInstances;
    }

    // Initialize feature weight vector to 0
    std::vector<double> fWeight(nFeatures, 0.0); // Feature weight
    double dist = 0;              // Distance of one instance to another (euclidean)
    double maxWeight = -INFINITY; // Feature's weights vector max value

    double nearFriendDist; // Distance to nearest friend
    int nearFriendIndex;   // Instance's position of nearest friend

    double nearEnemyDist;  // Distance to nearest enemy
    int nearEnemyIndex;    // Instance's position of nearest enemy

    for(int i = 0; i < nInstances; ++i) {

        nearFriendDist  = INFINITY;
        nearFriendIndex = 0;
        nearEnemyDist   = INFINITY;
        nearEnemyIndex  = 0;

        for(int j = 0; j < nInstances; ++j) {

            // Find nearest friend
            // If both instances belong to the same class
            if (trainingSet.second[j] == trainingSet.second[i] && j != i) {
                dist = distance(dsFeatures[i], dsFeatures[j], nFeatures);
                if (dist < nearFriendDist) {
                    nearFriendDist  = dist;
                    nearFriendIndex = j;
                }
            }

            // Find nearest enemy
            // If instances belong to different classes
            if (trainingSet.second[j] != trainingSet.second[i]) {
                dist = distance(dsFeatures[i], dsFeatures[j], nFeatures);
                if (dist < nearEnemyDist) {
                    nearEnemyDist  = dist;
                    nearEnemyIndex = j;
                }
            }
        }

        // Update feature weight vector

        double nearFriendDiff; // Difference between nearest friend and the
                               // i-th instance
        double nearEnemyDiff;  // Difference between nearest enemy and the
                               // i-th instance
        maxWeight = -INFINITY;
        cout << "[ ";
        for (int j = 0; j < nFeatures; ++j) {
            // The nearest friend is dsFeatures[nearFriendIndex]
            nearFriendDiff = pow(dsFeatures[nearFriendIndex][j] - dsFeatures[i][j], 2.0);
            // The nearest enemy is dsFeatures[nearEnemyIndex];
            nearEnemyDiff  = pow(dsFeatures[nearEnemyIndex][j]  - dsFeatures[i][j], 2.0);
            fWeight[j] = fWeight[j] - nearFriendDiff + nearEnemyDiff;
            cout << fWeight[j] << ", ";

            if (maxWeight < fWeight[j]) {
                maxWeight = fWeight[j];
            }
        }
        cout << "]" << endl;
    }

    // Normalize feature vector
    cout << "[ ";
    for(int j = 0; j < nFeatures; ++j) {
        if (fWeight[j] < 0.0) {
            fWeight[j] = 0.0;
        }
        else {
            fWeight[j] = fWeight[j]/maxWeight;
        }
        cout << fWeight[j] << ", ";
    }
    cout << "]" << endl;
    return fWeight;
}

#endif // RELIEF_HPP_
