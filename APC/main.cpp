/**
 *  Universidad Simon Bolivar
 *  CI-5652 - Diseño de Algoritmos II
 *  @author : Erick Silva
 *  @author : Stefani Castellanos 11-11394
 *
 *  Functions for reading the dataset
 */

#include <string>
#include <ctime>
#include "dataset.hpp"
#include "relief.hpp"
#include "NearestNeighbor.cpp"
#include "ILS.hpp"

int NUM_PARTITIONS = 4;
std::vector<string> dataNames = {"iris" , "sonar", "spambase", "wdbc" };

int main(int argc, char const *argv[]) {

    time_t timeStart;
    time_t timeEnd;
    double timeElapsed;
    double nHits, nError;
    string header = "Particion, Aciertos(\%) , Error(\%), Tiempo(sg) ";

    // Statistics
    for (int name = 0; name < NUM_PARTITIONS; ++name) {

        // File statistics for no weight
        ofstream fileNoW;
        fileNoW.open("../Paper/statistics/" + dataNames[name] + "/no_weights.csv");
        fileNoW << header << std::endl;

        // File statistics for RELIEF
        ofstream fileR;
        fileR.open("../Paper/statistics/" + dataNames[name] + "/relief.csv");
        fileR << header << std::endl;

        std::string dsFile = "datasets/" + dataNames[name] + "/" + dataNames[name] + ".data";
        DataSet dataset = readFile(dsFile.c_str());

        for (int i = 1; i < NUM_PARTITIONS + 1; ++i) {
          std::pair<DataSet, DataSet> ds = dataset.makePartition(i*10, 0.6);

          // Statistics without weights
          std::cout << "Executing K-NN algorithm without weights on " << dataNames[name] << " dataset (" << i << ")" << std::endl;
          timeStart = time(NULL);
          std::vector<double> weights1(ds.first.nFeatures, 1.0);
          nHits = NN1(ds.first, ds.second, weights1);
          timeEnd = time(NULL);
          timeElapsed = difftime(timeEnd, timeStart);
          nError = 100 - nHits;
          fileNoW << i << ", " << nHits << ", " << nError << ", " << timeElapsed << std::endl;
          std::cout << "done!" << std::endl;

          // Statistics RELIEF
          std::cout << "Executing RELIEF algorithm on " << dataNames[name] << " dataset (" << i << ")" << std::endl;
          timeStart = time(NULL);
          std::vector<double> weights2 = relief(ds.first);
          nHits = NN1(ds.first, ds.second, weights2);
          timeEnd = time(NULL);
          timeElapsed = difftime(timeEnd, timeStart);
          nError = 100 - nHits;
          fileR << i << ", " << nHits << ", " << nError << ", " << timeElapsed << std::endl;
          std::cout << "done!" << std::endl;
        }
        fileR.close();
    }


    return 0;
}
