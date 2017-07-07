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
int NUM_DATASETS = 4;
std::vector<string> dataNames = {"iris", "sonar", "wdbc", "spambase" };
// std::vector<int> maxIterations = { 2, 150, 15, 2 };
// std::vector<int> maxIterations = { 5, 250, 40, 10 };
std::vector<int> maxIterations = { 10, 350, 50, 15 };
// std::vector<int> maxIterations = { 50, 500, 100, 20 };
// std::vector<int> maxIterations = { 100, 1, 150, 1 };
std::vector<int> neighborsPerGen = { 3, 80, 10, 3 };
// std::vector<int> neighborsPerGen = { 4, 150, 20, 5 };
// std::vector<int> neighborsPerGen = { 8, 250, 30, 10 };
// std::vector<int> neighborsPerGen = { 20, 450, 60, 15 };
// std::vector<int> neighborsPerGen = { 60, 1, 80, 1 };
// std::vector<int> neighborsPerGenSA = { 2, 2, 2, 2 };
std::vector<int> neighborsPerGenSA = { 4, 4, 4, 4 };
// std::vector<int> neighborsPerGenSA = { 8, 8, 8, 8 };
// std::vector<int> neighborsPerGenSA = { 16, 16, 16, 16 };
// std::vector<int> maxIterationsWithoutChange = { 2, 2, 2, 2 };
// std::vector<int> maxIterationsWithoutChange = { 4, 4, 4, 4 };
std::vector<int> maxIterationsWithoutChange = { 6, 6, 6, 6 };
// std::vector<int> maxIterationsWithoutChange = { 8, 8, 8, 8 };
std::vector<int> temperature = { 50, 50, 50, 50 };
std::vector<int> internalIter = { 100, 100, 100, 100 };
// std::vector<int> popSize = { 5, 5, 5, 5};
std::vector<int> popSize = { 10, 10, 10, 10};
// std::vector<int> popSize = { 20, 20, 20, 20};
// std::vector<int> popSize = { 40, 40, 40, 40};
// std::vector<double> CR = { 0.3, 0.3, 0.3, 0.3 };
std::vector<double> CR = { 0.5, 0.5, 0.5, 0.5 };
// std::vector<double> CR = { 0.7, 0.7, 0.7, 0.7 };
// std::vector<double> F = { 0.3, 0.3, 0.3, 0.3 };
// std::vector<double> F = { 0.5, 0.5, 0.5, 0.5 };
std::vector<double> F = { 0.7, 0.7, 0.7, 0.7 };
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

    Statistics(std::string fileName, int name, int size){
        file.open("../Paper/statistics/" + dataNames[name] + "/" + fileName + ".csv");
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
        this-> file << solution.weights[size-1] << " ]" << std::endl;
    }
};

// Statistics without weights
void statisticsNoWeight( Statistics &stats,
                         int name,
                         int part,
                         std::pair<DataSet, DataSet> &ds ) {

    std::cout << "Executing K-NN algorithm without weights on " << dataNames[name]
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
                               int name,
                               int part,
                               std::pair<DataSet, DataSet> &ds) {

    std::cout << "Executing RELIEF algorithm on " << dataNames[name] << " dataset (" << part << ")" << std::endl;
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
                      int name,
                      int part,
                      std::pair<DataSet, DataSet> &ds ) {

    std::cout << "Executing LS (random) algorithm on " << dataNames[name] << " dataset (" << part << ")" << std::endl;
    timeStart = clock();
    APC_Instance w(ds.first.nFeatures);
    APC_Instance weights = localSearch(w, ds.first, ds.second, maxIterations[name], neighborsPerGen[name]);
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
                          int name,
                          int part,
                          std::pair<DataSet, DataSet> &ds,
                          APC_Instance reliefV) {

    std::cout << "Executing LS (RELIEF) algorithm on " << dataNames[name] << " dataset (" << part << ")" << std::endl;
    timeStart = clock();
    APC_Instance w(reliefV);
    APC_Instance weights = localSearch(w, ds.first, ds.second, maxIterations[name], neighborsPerGen[name]);
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
                      int name,
                      int part,
                      std::pair<DataSet, DataSet> &ds ) {

    std::cout << "Executing ILS (random) algorithm on " << dataNames[name] << " dataset (" << part << ")" << std::endl;
    timeStart = clock();
    APC_Instance w(ds.first.nFeatures);
    APC_Instance weights = ILS_convergence(w, ds.first, ds.second, maxIterations[name], neighborsPerGen[name], maxIterationsWithoutChange[name]);
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
                          int name,
                          int part,
                          std::pair<DataSet, DataSet> &ds,
                          APC_Instance reliefV) {

    std::cout << "Executing ILS (RELIEF) algorithm on " << dataNames[name] << " dataset (" << part << ")" << std::endl;
    timeStart = clock();
    APC_Instance w(reliefV);
    APC_Instance weights = ILS_convergence(w, ds.first, ds.second, maxIterations[name], neighborsPerGen[name], maxIterationsWithoutChange[name]);
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
                      int name,
                      int part,
                      std::pair<DataSet, DataSet> &ds ) {

    std::cout << "Executing SA (random) algorithm on " << dataNames[name] << " dataset (" << part << ")" << std::endl;
    timeStart = clock();
    APC_Instance w(ds.first.nFeatures);
    APC_Instance weights = simulatedAnnealing(w, ds.first, ds.second, maxIterations[name], temperature[name], neighborsPerGenSA[name], internalIter[name]);
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
                          int name,
                          int part,
                          std::pair<DataSet, DataSet> &ds,
                          APC_Instance reliefV) {

    std::cout << "Executing SA (RELIEF) algorithm on " << dataNames[name] << " dataset (" << part << ")" << std::endl;
    timeStart = clock();
    APC_Instance w(reliefV);
    APC_Instance weights = simulatedAnnealing(w, ds.first, ds.second, maxIterations[name], temperature[name], neighborsPerGenSA[name], internalIter[name]);
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
                      int name,
                      int part,
                      std::pair<DataSet, DataSet> &ds ) {

    std::cout << "Executing DE (random) algorithm on " << dataNames[name] << " dataset (" << part << ")" << std::endl;
    timeStart = clock();
    APC_Instance weights = difEvolution(ds.first.nFeatures, ds.first, ds.second, popSize[name], CR[name], F[name], maxIterations[name], 2);
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
                           int name,
                           int part,
                           std::pair<DataSet, DataSet> &ds ) {

    std::cout << "Executing Scatter (random) algorithm on " << dataNames[name] << " dataset (" << part << ")" << std::endl;
    timeStart = clock();
    APC_Instance w(ds.first.nFeatures);
    APC_Instance weights = scatter(w, ds.first, ds.second, popSize[name], maxIterations[name], maxIterationsWithoutChange[name]);
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
                              int name,
                              int part,
                              std::pair<DataSet, DataSet> &ds,
                              APC_Instance reliefV) {

    std::cout << "Executing Scatter (RELIEF) algorithm on " << dataNames[name] << " dataset (" << part << ")" << std::endl;
    timeStart = clock();
    APC_Instance w(reliefV);
    APC_Instance weights = scatter(w, ds.first, ds.second, popSize[name], maxIterations[name], maxIterationsWithoutChange[name]);
    nHits = weights.evaluate(ds.first, ds.second);
    timeEnd = clock();
    timeElapsed = difftime(timeEnd, timeStart)/CLOCKS_PER_SEC;
    nError = 100 - nHits;
    stats.update(nHits, nError, timeElapsed, weights, ds.first.nFeatures);
    stats.file << part << ", " << nHits << ", " << nError << ", " << timeElapsed << std::endl;
    std::cout << "done!" << std::endl;

}

int main(int argc, char const *argv[]) {

    // Statistics
    if (strcmp(argv[1], "all") == 0) {

        for (int name = 0; name < NUM_DATASETS; ++name) {
            // Read dataset file
            std::string dsFile = "datasets/" + dataNames[name] + "/" + dataNames[name] + ".data";
            DataSet dataset = readFile(dsFile.c_str());

            Statistics noW("no_weights", name, dataset.nFeatures);
            Statistics rel("relief", name, dataset.nFeatures);
            Statistics LS("LS_random", name, dataset.nFeatures);
            Statistics LS_r("LS_relief", name, dataset.nFeatures);
            Statistics ILS("ILS_random", name, dataset.nFeatures);
            Statistics ILS_r("ILS_relief", name, dataset.nFeatures);
            Statistics SA("SA_random", name, dataset.nFeatures);
            Statistics SA_r("SA_relief", name, dataset.nFeatures);
            Statistics DE("DE", name, dataset.nFeatures);
            Statistics Scatter("Scatter_random", name, dataset.nFeatures);
            Statistics Scatter_r("Scatter_relief", name, dataset.nFeatures);


            for (int i = 1; i < NUM_PARTITIONS + 1; ++i) {
                std::pair<DataSet, DataSet> ds = dataset.makePartition(i*10, 0.6);
                statisticsNoWeight (noW, name, i, ds);
                APC_Instance reliefV = statisticsRelief (rel, name, i, ds);
                statisticsLSRand (ILS, name, i, ds);
                statisticsLSRelief (ILS_r, name, i, ds, reliefV);
                statisticsILSRand (ILS, name, i, ds);
                statisticsILSRelief (ILS_r, name, i, ds, reliefV);
                statisticsSARand (SA, name, i, ds);
                statisticsSARelief (SA_r, name, i, ds, reliefV);
                statisticsDERand (DE, name, i, ds);
                statisticsScatterRand(Scatter, name, i, ds);
                statisticsScatterRelief(Scatter_r, name, i, ds, reliefV);
            }
            noW.average(dataset.nFeatures);
            rel.average(dataset.nFeatures);
            LS.average(dataset.nFeatures);
            LS_r.average(dataset.nFeatures);
            ILS.average(dataset.nFeatures);
            ILS_r.average(dataset.nFeatures);
            SA.average(dataset.nFeatures);
            SA_r.average(dataset.nFeatures);
            DE.average(dataset.nFeatures);
            Scatter.average(dataset.nFeatures);
            Scatter_r.average(dataset.nFeatures);

            // Close all files
            noW.file.close();
            rel.file.close();
            LS.file.close();
            LS_r.file.close();
            ILS.file.close();
            ILS_r.file.close();
            SA.file.close();
            SA_r.file.close();
            DE.file.close();
            Scatter.file.close();
            Scatter_r.file.close();
        }
    }
    else {
        for (int name = 0; name < NUM_DATASETS; ++name) {
            // Read dataset file
            std::string dsFile = "datasets/" + dataNames[name] + "/" + dataNames[name] + ".data";
            DataSet dataset = readFile(dsFile.c_str());

            Statistics stats(argv[1], name, dataset.nFeatures);

            for (int i = 1; i < NUM_PARTITIONS + 1; ++i) {
                std::pair<DataSet, DataSet> ds = dataset.makePartition(i*10, 0.6);

                if (strcmp(argv[1], "no_weights") == 0) {
                    statisticsNoWeight (stats, name, i, ds);
                }
                else if (strcmp(argv[1], "relief") == 0) {
                    APC_Instance reliefV = statisticsRelief (stats, name, i, ds);
                }
                else if (strcmp(argv[1], "LS_random") == 0) {
                    statisticsLSRand(stats, name, i, ds);
                }
                else if (strcmp(argv[1], "LS_relief") == 0) {
                    std::vector<double> weights = relief(ds.first);
                    statisticsLSRelief(stats, name, i, ds, weights);
                }
                else if (strcmp(argv[1], "ILS_random") == 0) {
                    statisticsILSRand (stats, name, i, ds);
                }
                else if (strcmp(argv[1], "ILS_relief") == 0) {
                    std::vector<double> weights = relief(ds.first);
                    statisticsILSRelief (stats, name, i, ds, weights);
                }
                else if (strcmp(argv[1], "SA_random") == 0) {
                    statisticsSARand(stats, name, i, ds);
                }
                else if (strcmp(argv[1], "SA_relief") == 0) {
                    std::vector<double> weights = relief(ds.first);
                    statisticsSARelief(stats, name, i, ds, weights);
                }
                else if (strcmp(argv[1], "DE") == 0) {
                    statisticsDERand(stats, name, i, ds);
                }
                else if (strcmp(argv[1], "Scatter_random") == 0) {
                    statisticsScatterRand(stats, name, i, ds);
                }
                else if (strcmp(argv[1], "Scatter_relief") == 0) {
                    std::vector<double> weights = relief(ds.first);
                    statisticsScatterRelief(stats, name, i, ds, weights);
                }
            }

            stats.average(dataset.nFeatures);
            stats.file.close();

            std::cout << "maxIter, popSize, CR, F, Aciertos, Tiempo" << std::endl;
            std::cout << maxIterations[name] << ", " << popSize[name] << ", " << CR[name] << ", " << F[name] << ", ";
            // std::cout << "maxIter, popSize, maxIterWC, Aciertos, Tiempo" << std::endl;
            // std::cout << maxIterations[name] << ", " << popSize[name] << ", " << maxIterationsWithoutChange[name] << ", ";
            // std::cout << std::endl << "maxIter, NumGen, temp, estable, Aciertos, Tiempo" << std::endl;
            // std::cout << maxIterations[name] << ", " << neighborsPerGenSA[name] << ", " << temperature[name] << ", " << internalIter[name] << ", ";
            std::cout << stats.hits << ", " << stats.time << std::endl << std::endl;
        }
    }
    return 0;
}
