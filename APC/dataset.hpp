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
};

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
        std::cout << "number or rows: " << numRows << " number of columns " << numCols << std::endl;

        // Converting instances to double
        for (int i = 0; i < numRows; ++i) {
            std:: vector<double> fVector;
            for (int j = 0; j < numCols - 1; j++) {
                std::cout << features[i][j] << ", ";
                fVector.push_back(stod(features[i][j]));
            }
            std::cout << result[i] << std::endl;
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
