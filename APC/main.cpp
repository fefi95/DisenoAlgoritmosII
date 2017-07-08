/**
 *  Universidad Simon Bolivar
 *  CI-5652 - Diseño de Algoritmos II
 *  @author : Erick Silva
 *  @author : Stefani Castellanos 11-11394
 *
 *  Statistics for APC problem
 */

#include <string>
#include <string.h>
#include <ctime>
#include "dataset.hpp"
#include "relief.hpp"
#include "NearestNeighbor.cpp"
#include "local_search.cpp"
#include "ILS.hpp"
#include "SimAnnealing.cpp"
#include "scatter.cpp"
#include "diffEvolution.cpp"

int NUM_PARTITIONS = 2;
int NUM_DATASETS = 1;
int maxIterations, neighborsPerGen, maxIterationsWithoutChange = 0;
int temperature, internalIter, popSize = 0;
double CR, F = 0.0;
clock_t timeStart;
clock_t timeEnd;
double timeElapsed;
double nHits, nError;
string header = "Particion, Aciertos(\%) , Error(\%), Tiempo(sg) ";

class Statistics {
public:
    ofstream file;
    double hits;
    double miss;
    double time;
    APC_Instance solution;

    Statistics(std::string fileName, std::string name, int size){
        file.open("../Paper/statistics/" + name + "/" + fileName + ".csv");
        file << header << std::endl;
        hits = 0;
        miss = 0;
        time = 0.0;
        std::vector<double> fWeight(size, 0.0);
        solution.weights = fWeight;
    };

    void update(double nHits, double nMiss, double timeE, APC_Instance &sol, int size) {
        hits += nHits;
        miss += nMiss;
        time += timeE;
        for (int i = 0; i < size; ++i) {
            solution.weights[i] += sol.weights[i];
        }
    }

    void average(int size) {
        this-> hits = this-> hits/NUM_PARTITIONS;
        this-> miss = this-> miss/NUM_PARTITIONS;
        this-> time = this-> time/NUM_PARTITIONS;
        this-> file << "promedio, "  << this-> hits << ", " << this-> miss << ", " << this-> time << std::endl;;
        this-> file << "solucion: [ ";
        for (int i = 0; i < size-1; ++i) {
            solution.weights[i] = solution.weights[i]/NUM_PARTITIONS;
            this-> file << solution.weights[i] << ", ";
        }
        this-> file << solution.weights[size-1]/NUM_PARTITIONS << " ]" << std::endl;
    }
};

// Statistics without weights
void statisticsNoWeight( Statistics &stats,
                         std::string name,
                         int part,
                         std::pair<DataSet, DataSet> &ds ) {

    std::cout << "Executing K-NN algorithm without weights on " << name
              << " dataset (" << part << ")" << std::endl;
    timeStart = clock();
    std::vector<double> weights(ds.first.nFeatures, 1.0);
    nHits = NN1(ds.first, ds.second, weights);
    timeEnd = clock();
    timeElapsed = difftime(timeEnd, timeStart)/CLOCKS_PER_SEC;
    nError = 100 - nHits;
    APC_Instance w(weights);
    stats.update(nHits, nError, timeElapsed, w, ds.first.nFeatures);
    stats.file << part << ", " << nHits << ", " << nError << ", " << timeElapsed << std::endl;
    std::cout << "done!" << std::endl;
}

// Statistics RELIEF
APC_Instance statisticsRelief( Statistics &stats,
                               std::string name,
                               int part,
                               std::pair<DataSet, DataSet> &ds) {

    std::cout << "Executing RELIEF algorithm on " << name << " dataset (" << part << ")" << std::endl;
    timeStart = clock();
    std::vector<double> weights = relief(ds.first);
    nHits = NN1(ds.first, ds.second, weights);
    timeEnd = clock();
    timeElapsed = difftime(timeEnd, timeStart)/CLOCKS_PER_SEC;
    nError = 100 - nHits;
    APC_Instance w(weights);
    stats.update(nHits, nError, timeElapsed, w, ds.first.nFeatures);
    stats.file << part << ", " << nHits << ", " << nError << ", " << timeElapsed << std::endl;
    std::cout << "done!" << std::endl;

    return weights;
}

// Statistics LS random
void statisticsLSRand(Statistics &stats,
                      std::string name,
                      int part,
                      std::pair<DataSet, DataSet> &ds ) {

    std::cout << "Executing LS (random) algorithm on " << name << " dataset (" << part << ")" << std::endl;
    timeStart = clock();
    APC_Instance w(ds.first.nFeatures);
    APC_Instance weights = localSearch(w, ds.first, ds.second, maxIterations, neighborsPerGen);
    nHits = weights.evaluate(ds.first, ds.second);
    timeEnd = clock();
    timeElapsed = difftime(timeEnd, timeStart)/CLOCKS_PER_SEC;
    nError = 100 - nHits;
    stats.update(nHits, nError, timeElapsed, weights, ds.first.nFeatures);
    stats.file << part << ", " << nHits << ", " << nError << ", " << timeElapsed << std::endl;
    std::cout << "done!" << std::endl;

}

// Statistics LS relief
void statisticsLSRelief( Statistics &stats,
                          std::string name,
                          int part,
                          std::pair<DataSet, DataSet> &ds,
                          APC_Instance reliefV) {

    std::cout << "Executing LS (RELIEF) algorithm on " << name << " dataset (" << part << ")" << std::endl;
    timeStart = clock();
    APC_Instance w(reliefV);
    APC_Instance weights = localSearch(w, ds.first, ds.second, maxIterations, neighborsPerGen);
    nHits = weights.evaluate(ds.first, ds.second);
    timeEnd = clock();
    timeElapsed = difftime(timeEnd, timeStart)/CLOCKS_PER_SEC;
    nError = 100 - nHits;
    stats.update(nHits, nError, timeElapsed, weights, ds.first.nFeatures);
    stats.file << part << ", " << nHits << ", " << nError << ", " << timeElapsed << std::endl;
    std::cout << "done!" << std::endl;

}

// Statistics ILS random
void statisticsILSRand(Statistics &stats,
                      std::string name,
                      int part,
                      std::pair<DataSet, DataSet> &ds ) {

    std::cout << "Executing ILS (random) algorithm on " << name << " dataset (" << part << ")" << std::endl;
    timeStart = clock();
    APC_Instance w(ds.first.nFeatures);
    APC_Instance weights = ILS_convergence(w, ds.first, ds.second, maxIterations, neighborsPerGen, maxIterationsWithoutChange);
    nHits = weights.evaluate(ds.first, ds.second);
    timeEnd = clock();
    timeElapsed = difftime(timeEnd, timeStart)/CLOCKS_PER_SEC;
    nError = 100 - nHits;
    stats.update(nHits, nError, timeElapsed, weights, ds.first.nFeatures);
    stats.file << part << ", " << nHits << ", " << nError << ", " << timeElapsed << std::endl;
    std::cout << "done!" << std::endl;

}

// Statistics ILS relief
void statisticsILSRelief( Statistics &stats,
                          std::string name,
                          int part,
                          std::pair<DataSet, DataSet> &ds,
                          APC_Instance reliefV) {

    std::cout << "Executing ILS (RELIEF) algorithm on " << name
              << " dataset (" << part << ")" << std::endl;
    timeStart = clock();
    APC_Instance w(reliefV);
    APC_Instance weights = ILS_convergence(w, ds.first, ds.second, maxIterations, neighborsPerGen, maxIterationsWithoutChange);
    nHits = weights.evaluate(ds.first, ds.second);
    timeEnd = clock();
    timeElapsed = difftime(timeEnd, timeStart)/CLOCKS_PER_SEC;
    nError = 100 - nHits;
    stats.update(nHits, nError, timeElapsed, weights, ds.first.nFeatures);
    stats.file << part << ", " << nHits << ", " << nError << ", " << timeElapsed << std::endl;
    std::cout << "done!" << std::endl;

}

// Statistics SA random
void statisticsSARand(Statistics &stats,
                      std::string name,
                      int part,
                      std::pair<DataSet, DataSet> &ds ) {

    std::cout << "Executing SA (random) algorithm on " << name
              << " dataset (" << part << ")" << std::endl;
    timeStart = clock();
    APC_Instance w(ds.first.nFeatures);
    APC_Instance weights = simulatedAnnealing(w, ds.first, ds.second, maxIterations, temperature, neighborsPerGen, internalIter);
    nHits = weights.evaluate(ds.first, ds.second);
    timeEnd = clock();
    timeElapsed = difftime(timeEnd, timeStart)/CLOCKS_PER_SEC;
    nError = 100 - nHits;
    stats.update(nHits, nError, timeElapsed, weights, ds.first.nFeatures);
    stats.file << part << ", " << nHits << ", " << nError << ", " << timeElapsed << std::endl;
    std::cout << "done!" << std::endl;

}

// Statistics SA relief
void statisticsSARelief( Statistics &stats,
                          std::string name,
                          int part,
                          std::pair<DataSet, DataSet> &ds,
                          APC_Instance reliefV) {

    std::cout << "Executing SA (RELIEF) algorithm on " << name
              << " dataset (" << part << ")" << std::endl;
    timeStart = clock();
    APC_Instance w(reliefV);
    APC_Instance weights = simulatedAnnealing(w, ds.first, ds.second, maxIterations, temperature, neighborsPerGen, internalIter);
    nHits = weights.evaluate(ds.first, ds.second);
    timeEnd = clock();
    timeElapsed = difftime(timeEnd, timeStart)/CLOCKS_PER_SEC;
    nError = 100 - nHits;
    stats.update(nHits, nError, timeElapsed, weights, ds.first.nFeatures);
    stats.file << part << ", " << nHits << ", " << nError << ", " << timeElapsed << std::endl;
    std::cout << "done!" << std::endl;

}

// Statistics DE random
void statisticsDERand(Statistics &stats,
                      std::string name,
                      int part,
                      std::pair<DataSet, DataSet> &ds ) {

    std::cout << "Executing DE algorithm on " << name
              << " dataset (" << part << ")" << std::endl;
    timeStart = clock();
    APC_Instance weights = difEvolution(ds.first.nFeatures, ds.first, ds.second, popSize, CR, F, maxIterations, 2);
    nHits = weights.evaluate(ds.first, ds.second);
    timeEnd = clock();
    timeElapsed = difftime(timeEnd, timeStart)/CLOCKS_PER_SEC;
    nError = 100 - nHits;
    stats.update(nHits, nError, timeElapsed, weights, ds.first.nFeatures);
    stats.file << part << ", " << nHits << ", " << nError << ", " << timeElapsed << std::endl;
    std::cout << "done!" << std::endl;

}

// Statistics Scatter random
void statisticsScatterRand(Statistics &stats,
                           std::string name,
                           int part,
                           std::pair<DataSet, DataSet> &ds ) {

    std::cout << "Executing Scatter (random) algorithm on " << name
              << " dataset (" << part << ")" << std::endl;
    timeStart = clock();
    APC_Instance w(ds.first.nFeatures);
    APC_Instance weights = scatter(w, ds.first, ds.second, popSize, maxIterations, maxIterationsWithoutChange);
    nHits = weights.evaluate(ds.first, ds.second);
    timeEnd = clock();
    timeElapsed = difftime(timeEnd, timeStart)/CLOCKS_PER_SEC;
    nError = 100 - nHits;
    stats.update(nHits, nError, timeElapsed, weights, ds.first.nFeatures);
    stats.file << part << ", " << nHits << ", " << nError << ", " << timeElapsed << std::endl;
    std::cout << "done!" << std::endl;

}

// Statistics Scatter relief
void statisticsScatterRelief( Statistics &stats,
                              std::string name,
                              int part,
                              std::pair<DataSet, DataSet> &ds,
                              APC_Instance reliefV) {

    std::cout << "Executing Scatter (RELIEF) algorithm on " << name
              << " dataset (" << part << ")" << std::endl;
    timeStart = clock();
    APC_Instance w(reliefV);
    APC_Instance weights = scatter(w, ds.first, ds.second, popSize, maxIterations, maxIterationsWithoutChange);
    nHits = weights.evaluate(ds.first, ds.second);
    timeEnd = clock();
    timeElapsed = difftime(timeEnd, timeStart)/CLOCKS_PER_SEC;
    nError = 100 - nHits;
    stats.update(nHits, nError, timeElapsed, weights, ds.first.nFeatures);
    stats.file << part << ", " << nHits << ", " << nError << ", " << timeElapsed << std::endl;
    std::cout << "done!" << std::endl;

}

int main(int argc, char const *argv[]) {

    ifstream file(argv[1]);
    ofstream fileOut("output");

    if(file.is_open()) {

        std::string line;
        file >> NUM_PARTITIONS;

        while( !file.eof() ) {
            // Read file
            std::string name;
            file >> name;
            if (name.compare("") == 0) {
                return 0;
            }
            std::string function;
            file >> function;
            fileOut << name << " " << function << std::endl;

            if (function.compare("LS_random") == 0) {
                file >> maxIterations;
                file >> neighborsPerGen;
                fileOut << "maxIter, NumGen, Aciertos, Tiempo" << std::endl;
                fileOut << maxIterations << ", " << neighborsPerGen << ", ";
            }
            else if (function.compare("LS_relief") == 0) {
                file >> maxIterations;
                file >> neighborsPerGen;
                fileOut << "maxIter, NumGen, Aciertos, Tiempo" << std::endl;
                fileOut << maxIterations << ", " << neighborsPerGen << ", ";
            }
            else if (function.compare("ILS_random") == 0) {
                file >> maxIterations;
                file >> neighborsPerGen;
                file >> maxIterationsWithoutChange;
                fileOut << "maxIter, NumGen, maxIterWC, Aciertos, Tiempo" << std::endl;
                fileOut << maxIterations << ", " << neighborsPerGen << ", " << maxIterationsWithoutChange << ", ";
            }
            else if (function.compare("ILS_relief") == 0) {
                file >> maxIterations;
                file >> neighborsPerGen;
                file >> maxIterationsWithoutChange;
                fileOut << "maxIter, NumGen, maxIterWC, Aciertos, Tiempo" << std::endl;
                fileOut << maxIterations << ", " << neighborsPerGen << ", " << maxIterationsWithoutChange << ", ";
            }
            else if (function.compare("SA_random") == 0) {
                file >> maxIterations;
                file >> neighborsPerGen;
                file >> temperature;
                file >> internalIter;
                fileOut << "maxIter, NumGen, temp, estable, Aciertos, Tiempo" << std::endl;
                fileOut << maxIterations << ", " << neighborsPerGen << ", " << temperature << ", " << internalIter << ", ";
            }
            else if (function.compare("SA_relief") == 0) {
                file >> maxIterations;
                file >> neighborsPerGen;
                file >> temperature;
                file >> internalIter;
                fileOut << "maxIter, NumGen, temp, estable, Aciertos, Tiempo" << std::endl;
                fileOut << maxIterations << ", " << neighborsPerGen << ", " << temperature << ", " << internalIter << ", ";
            }
            else if (function.compare("DE") == 0) {
                file >> maxIterations;
                file >> popSize;
                file >> CR;
                file >> F;
                fileOut << "maxIter, popSize, maxIterWC, Aciertos, Tiempo" << std::endl;
                fileOut << maxIterations << ", " << popSize << ", " << CR << ", " << F << ", ";
            }
            else if (function.compare("Scatter_random") == 0) {
                file >> maxIterations;
                file >> popSize;
                file >> maxIterationsWithoutChange;
                fileOut << "maxIter, popSize, maxIterWC, Aciertos, Tiempo" << std::endl;
                fileOut << maxIterations << ", " << popSize << ", " << maxIterationsWithoutChange << ", ";
            }
            else if (function.compare("Scatter_relief") == 0) {
                file >> maxIterations;
                file >> popSize;
                file >> maxIterationsWithoutChange;
                fileOut << "maxIter, popSize, maxIterWC, Aciertos, Tiempo" << std::endl;
                fileOut << maxIterations << ", " << popSize << ", " << maxIterationsWithoutChange << ", ";
            }

            // Read dataset file
            std::string dsFile = "datasets/" + name + "/" + name + ".data";
            DataSet dataset = readFile(dsFile.c_str());

            Statistics stats(function.c_str(), name, dataset.nFeatures);

            for (int i = 1; i < NUM_PARTITIONS + 1; ++i) {
                std::pair<DataSet, DataSet> ds = dataset.makePartition(i*10, 0.6);

                if (function.compare("no_weights") == 0) {
                    statisticsNoWeight (stats, name, i, ds);
                }
                else if (function.compare("relief") == 0) {
                    APC_Instance reliefV = statisticsRelief (stats, name, i, ds);
                }
                else if (function.compare("LS_random") == 0) {
                    statisticsLSRand(stats, name, i, ds);
                }
                else if (function.compare("LS_relief") == 0) {
                    std::vector<double> weights = relief(ds.first);
                    statisticsLSRelief(stats, name, i, ds, weights);
                }
                else if (function.compare("ILS_random") == 0) {
                    statisticsILSRand (stats, name, i, ds);
                }
                else if (function.compare("ILS_relief") == 0) {
                    std::vector<double> weights = relief(ds.first);
                    statisticsILSRelief (stats, name, i, ds, weights);
                }
                else if (function.compare("SA_random") == 0) {
                    statisticsSARand(stats, name, i, ds);
                }
                else if (function.compare("SA_relief") == 0) {
                    std::vector<double> weights = relief(ds.first);
                    statisticsSARelief(stats, name, i, ds, weights);
                }
                else if (function.compare("DE") == 0) {
                    statisticsDERand(stats, name, i, ds);
                }
                else if (function.compare("Scatter_random") == 0) {
                    statisticsScatterRand(stats, name, i, ds);
                }
                else if (function.compare("Scatter_relief") == 0) {
                    std::vector<double> weights = relief(ds.first);
                    statisticsScatterRelief(stats, name, i, ds, weights);
                }
            }

            stats.average(dataset.nFeatures);
            stats.file.close();
            fileOut << stats.hits << ", " << stats.time << std::endl << std::endl;
        }
    }

    file.close();
    return 0;
}
