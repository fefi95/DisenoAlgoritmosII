/**
 *  Universidad Simon Bolivar
 *  CI-5652 - Diseño de Algoritmos II
 *  @author : Erick Silva
 *  @author : Stefani Castellanos 11-11394
 *
 *  Functions for reading the dataset
 */


#ifndef READER_HPP_
#define READER_HPP_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <vector>
using namespace std;

std::pair <vector<vector <string> >,vector <string> > readFile(const char* fileName){

    std::vector<vector <string> > features; //Feature vector
    std::vector<string>           result;   //Result vector
  	ifstream file(fileName);                //File to be read

  	//Make sure the file exists
  	if(file.is_open()) {

        int numRows = 0;   //Count of dataset's rows
        int numCols = 0;   //Count of dataset's columns
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

        //Print
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols-1; j++) {
                std::cout << features[i][j] << ", ";
            }
            std::cout << result[i] << std::endl;
        }
  	}

  	else {
        cout << "Unable to read specified file..." << endl;
    }
  	file.close();

    std::pair <vector<vector <string> >, vector <string> > dataset(features, result);
    return dataset;
}

#endif // READER_HPP_
