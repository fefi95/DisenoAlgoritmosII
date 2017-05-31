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
using namespace std;

void readFile(const char* fileName, int nRows){

  	ifstream file(fileName); //File to be read

  	//Make sure the file exists
  	if(file.is_open()) {
        int numRows = 0;
        int numCols = 0;
        std::string line;
        while(std::getline(file, line)) {
            ++numRows;
            std::stringstream  lineStream(line);
            std::string        cell;
            numCols = 0;
            while(std::getline(lineStream, cell, ',')) {
                ++numCols;
                std::cout << cell << ", ";
            }
            std::cout << "" << std::endl;
        }
        std::cout << "number or rows: " << numRows << " number of columns " << numCols << std::endl;
  	}

  	else {
      cout << "Unable to read specified file..." << endl;
    }
  	file.close();
}

#endif // READER_HPP_
