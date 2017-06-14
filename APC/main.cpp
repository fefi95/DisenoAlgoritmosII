/**
 *  Universidad Simon Bolivar
 *  CI-5652 - Diseño de Algoritmos II
 *  @author : Erick Silva
 *  @author : Stefani Castellanos 11-11394
 *
 *  Functions for reading the dataset
 */

#include "dataset.hpp"
#include "relief.hpp"
#include "NearestNeighbor.cpp"
#include <string>
#include <ctime>

int NUM_PARTITIONS = 4;
std::vector<string> dataNames = {"iris" , "sonar", "spambase", "wdbc" };

int main(int argc, char const *argv[]) {

    time_t timeStart;
    time_t timeEnd;
    double timeElapsed;
    double nHits, nError;

    // Statistics
    for (int name = 0; name < NUM_PARTITIONS; ++name) {
        ofstream file;
        file.open("../Paper/statistics/" + dataNames[name] + "/relief.csv");
        file << "Particion, Aciertos() , Error(), Tiempo(sg) " << std::endl;
        std::string dsFile = "datasets/" + dataNames[name] + "/" + dataNames[name] + ".data";
        DataSet dataset = readFile(dsFile.c_str());

        // Statistics RELIEF
        for (int i = 1; i < NUM_PARTITIONS + 1; ++i) {
          std::cout << "Executing RELIEF algorithm on " << dataNames[name] << " dataset (" << i << ")" << std::endl;
          std::pair<DataSet, DataSet> ds = dataset.makePartition(i*10, 0.6);
          // std::cout << ds.first << std::endl;
          timeStart = time(NULL);
          std::vector<double> weights = relief(ds.first);
          nHits = NN1(ds.first, ds.second, weights);
          timeEnd = time(NULL);

          timeElapsed = difftime(timeEnd, timeStart);
          nError = 100 - nHits;
          file << i << ", " << nHits << ", " << nError << ", " << timeElapsed << std::endl;
          std::cout << "done!" << std::endl;
        }
        file.close();
    }


    return 0;
}
