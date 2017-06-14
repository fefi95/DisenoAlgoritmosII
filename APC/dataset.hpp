/**
 *  Universidad Simon Bolivar
 *  CI-5652 - Diseño de Algoritmos II
 *  @author : Erick Silva
 *  @author : Stefani Castellanos 11-11394
 *
 *  Functions for reading the dataset
 */


#ifndef DATASET_HPP
#define DATASET_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <random>
using namespace std;

/**
 * Class to store datasets components
 *
 */
class DataSet {
public:
    std::vector<vector <double> > instances;
    std::vector< std::string > result;
    int nInstances;
    int nFeatures;

    /**
     * Constructor
     *
     * @params inst : Instances of the dataset
     * @params res  : Class of every instances
     * @params m    : Number of instances
     * @params n    : Number of features
     *
     */
    DataSet(std::vector<vector <double> > inst,
            std::vector< std::string > res,
            int m,
            int n
           ) {
        instances = inst;
        result = res;
        nInstances = m;
        nFeatures = n;
    }

    /**
     * Function that makes a partition of the dataset
     * given a percentage of the training set size.
     * (i.e percentT = 0.6 makes a 60-40 partition.
     *
     * @params seed      : seed for random number generation
     * @params percentT  : percentage of the trainig set's size.
     *                     a number between [0, 1]
     *
     */
    std::pair<DataSet, DataSet> makePartition(int seed, float percentT) {

        // index vector for shuffleling
        std::vector<int> index;
        for (int i = 0; i < nInstances; ++i) {
            index.push_back(i);
        }
        std::shuffle(std::begin(index), std::end(index), std::default_random_engine(seed));

        // Trainning set
        std::vector<vector <double> > instTrain;
        std::vector< std::string > resTrain;
        int nTrain = nInstances*percentT;
        for (int i = 0; i < nTrain; ++i) {
            instTrain.push_back(instances[index[i]]);
            resTrain.push_back(result[index[i]]);
        }
        DataSet trainingSet(instTrain, resTrain, nTrain, nFeatures);
        // trainingSet.print(cout);

        // Test set
        std::vector<vector <double> > instTest;
        std::vector< std::string > resTest;
        int nTest = nInstances - nInstances*percentT;
        for (int i = nTrain; i < nTest + nTrain; ++i) {
            instTest.push_back(instances[index[i]]);
            resTest.push_back(result[index[i]]);
        }
        DataSet testSet(instTest, resTest, nTest, nFeatures);
        // testSet.print(cout);

        std::pair<DataSet, DataSet> dataset(trainingSet, testSet);
        return dataset;
    }

    // Print
    inline void print(std::ostream &os) const {

        os << "Number of instances(examples): " << nInstances << std::endl;
        os << "Number of features: " << nFeatures << std::endl;

        for (int i = 0; i < nInstances; ++i) {
            for (int j = 0; j < nFeatures; ++j) {
                os << instances[i][j] << ", ";
            }
            os << result[i] << std::endl ;
        }
    }
};

/**
 * Overriding << operator for DataSet class
 *
 */
inline std::ostream& operator<<(std::ostream &os, const DataSet &dataset) {
    dataset.print(os);
    return os;
}

/**
 * Functions thar reads a file containing a dataset.
 * All of the features of the dataset must be numbers.
 * The class of every instances can be a string.
 *
 * @params inst : Instances of the dataset
 *
 * @return a DataSet with the elements of the read dataset
 */

DataSet readFile(const char* fileName){

    std::vector<vector <string> > features;  //Feature vector
    std::vector<vector <double> > instances; //Feature vector
    std::vector<string>           result;    //Result vector
  	ifstream file(fileName);                 //File to be read
    int numRows = 0;   //Count of dataset's rows
    int numCols = 0;   //Count of dataset's columns

    //Make sure the file exists
    if(file.is_open()) {

        std::string line;

        while(std::getline(file, line)) {
            std::stringstream  lineStream(line);
            std::string        cell;
            std::vector<string> featureRow;

            ++numRows;
            numCols = 0;

            while(std::getline(lineStream, cell, ',')) {
                ++numCols;
                featureRow.push_back(cell);
            }
            //Delete last cell of the feature row a add it to the result vector
            featureRow.pop_back();
            result.push_back(cell);
            features.push_back(featureRow);
        }
        // std::cout << "number or rows: " << numRows << " number of columns " << numCols << std::endl;

        // Converting instances to double
        for (int i = 0; i < numRows; ++i) {
            std:: vector<double> fVector;
            for (int j = 0; j < numCols - 1; j++) {
                // std::cout << features[i][j] << ", ";
                fVector.push_back(stod(features[i][j]));
            }
            // std::cout << result[i] << std::endl;
            instances.push_back(fVector);
        }
    }

    else {
        cout << "Unable to read specified file..." << endl;
    }
    file.close();

    DataSet dataset(instances, result, numRows, numCols-1);
    return dataset;
}

#endif // DATASET_HPP
