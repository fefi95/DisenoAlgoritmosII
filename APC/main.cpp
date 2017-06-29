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
#include "ILS.hpp"
#include "SimAnnealing.cpp"
#include "scatter.cpp"

int NUM_PARTITIONS = 5;
int NUM_DATASETS = 4;
std::vector<string> dataNames = {"iris", "sonar", "wdbc", "spambase" };
std::vector<int> maxIterations = {10, 150, 15, 2 };
std::vector<int> neighborsPerGen = {8, 80, 10, 3 };
std::vector<int> temperature = {3, 3, 5, 5 };
time_t timeStart;
time_t timeEnd;
double timeElapsed;
double nHits, nError;
string header = "Particion, Aciertos(\%) , Error(\%), Tiempo(sg) ";

class Statistics {
public:
    ofstream file;
    double hits;
    double miss;
    double time;

    Statistics(std::string fileName, int name){
        file.open("../Paper/statistics/" + dataNames[name] + "/" + fileName + ".csv");
        file << header << std::endl;
        hits = 0;
        miss = 0;
        time = 0.0;
    };

    void update(double nHits, double nMiss, double timeE) {
        hits += nHits;
        miss += nMiss;
        time += timeE;
    }
};

// Statistics without weights
void statisticsNoWeight( Statistics &stats,
                         int name,
                         int part,
                         std::pair<DataSet, DataSet> &ds ) {

    std::cout << "Executing K-NN algorithm without weights on " << dataNames[name]
              << " dataset (" << part << ")" << std::endl;
    timeStart = time(NULL);
    std::vector<double> weights(ds.first.nFeatures, 1.0);
    nHits = NN1(ds.first, ds.second, weights);
    timeEnd = time(NULL);
    timeElapsed = difftime(timeEnd, timeStart);
    nError = 100 - nHits;
    stats.update(nHits, nError, timeElapsed);
    stats.file << part << ", " << nHits << ", " << nError << ", " << timeElapsed << std::endl;
    std::cout << "done!" << std::endl;
}

// Statistics RELIEF
APC_Instance statisticsRelief( Statistics &stats,
                               int name,
                               int part,
                               std::pair<DataSet, DataSet> &ds) {

    std::cout << "Executing RELIEF algorithm on " << dataNames[name] << " dataset (" << part << ")" << std::endl;
    timeStart = time(NULL);
    std::vector<double> weights = relief(ds.first);
    nHits = NN1(ds.first, ds.second, weights);
    timeEnd = time(NULL);
    timeElapsed = difftime(timeEnd, timeStart);
    nError = 100 - nHits;
    stats.update(nHits, nError, timeElapsed);
    stats.file << part << ", " << nHits << ", " << nError << ", " << timeElapsed << std::endl;
    std::cout << "done!" << std::endl;

    return weights;
}

// Statistics ILS random
void statisticsILSRand(Statistics &stats,
                      int name,
                      int part,
                      std::pair<DataSet, DataSet> &ds ) {

    std::cout << "Executing ILS (random) algorithm on " << dataNames[name] << " dataset (" << part << ")" << std::endl;
    timeStart = time(NULL);
    APC_Instance w(ds.first.nFeatures);
    APC_Instance weights = ILS_convergence(w, ds.first, ds.second, maxIterations[name], neighborsPerGen[name], 2);
    nHits = weights.evaluate(ds.first, ds.second);
    timeEnd = time(NULL);
    timeElapsed = difftime(timeEnd, timeStart);
    nError = 100 - nHits;
    stats.update(nHits, nError, timeElapsed);
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
    timeStart = time(NULL);
    APC_Instance w(reliefV);
    APC_Instance weights = ILS_convergence(w, ds.first, ds.second, maxIterations[name], neighborsPerGen[name], 2);
    nHits = weights.evaluate(ds.first, ds.second);
    timeEnd = time(NULL);
    timeElapsed = difftime(timeEnd, timeStart);
    nError = 100 - nHits;
    stats.update(nHits, nError, timeElapsed);
    stats.file << part << ", " << nHits << ", " << nError << ", " << timeElapsed << std::endl;
    std::cout << "done!" << std::endl;

}

// Statistics SA random
void statisticsSARand(Statistics &stats,
                      int name,
                      int part,
                      std::pair<DataSet, DataSet> &ds ) {

    std::cout << "Executing SA (random) algorithm on " << dataNames[name] << " dataset (" << part << ")" << std::endl;
    timeStart = time(NULL);
    APC_Instance w(ds.first.nFeatures);
    APC_Instance weights = simulatedAnnealing(w, ds.first, ds.second, maxIterations[name], temperature[name], neighborsPerGen[name]);
    nHits = weights.evaluate(ds.first, ds.second);
    timeEnd = time(NULL);
    timeElapsed = difftime(timeEnd, timeStart);
    nError = 100 - nHits;
    stats.update(nHits, nError, timeElapsed);
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
    timeStart = time(NULL);
    APC_Instance w(reliefV);
    APC_Instance weights = simulatedAnnealing(w, ds.first, ds.second, maxIterations[name], temperature[name], neighborsPerGen[name]);
    nHits = weights.evaluate(ds.first, ds.second);
    timeEnd = time(NULL);
    timeElapsed = difftime(timeEnd, timeStart);
    nError = 100 - nHits;
    stats.update(nHits, nError, timeElapsed);
    stats.file << part << ", " << nHits << ", " << nError << ", " << timeElapsed << std::endl;
    std::cout << "done!" << std::endl;

}

int main(int argc, char const *argv[]) {

    // Statistics
    if (strcmp(argv[1], "all") == 0) {

        for (int name = 0; name < NUM_DATASETS; ++name) {
            Statistics noW("no_weights", name);
            Statistics rel("relief", name);
            Statistics ILS("ILS_random", name);
            Statistics ILS_r("ILS_relief", name);
            Statistics SA("SA_random", name);
            Statistics SA_r("SA_relief", name);

            // Read dataset file
            std::string dsFile = "datasets/" + dataNames[name] + "/" + dataNames[name] + ".data";
            DataSet dataset = readFile(dsFile.c_str());

            for (int i = 1; i < NUM_PARTITIONS + 1; ++i) {
                std::pair<DataSet, DataSet> ds = dataset.makePartition(i*10, 0.6);
                statisticsNoWeight (noW, name, i, ds);
                APC_Instance reliefV = statisticsRelief (rel, name, i, ds);
                statisticsILSRand (ILS, name, i, ds);
                statisticsILSRelief (ILS_r, name, i, ds, reliefV);
                statisticsSARand (SA, name, i, ds);
                statisticsSARelief (SA_r, name, i, ds, reliefV);
            }
            // Average
            double ave_hits, ave_miss, ave_time;

            ave_hits = noW.hits/NUM_PARTITIONS; ave_miss = noW.miss/NUM_PARTITIONS; ave_time = noW.time/NUM_PARTITIONS;
            noW.file << "promedio, "  << ave_hits << ", " << ave_miss << ", " << ave_time << std::endl;

            ave_hits = rel.hits/NUM_PARTITIONS; ave_miss = rel.miss/NUM_PARTITIONS; ave_time = rel.time/NUM_PARTITIONS;
            rel.file << "promedio, "  << ave_hits << ", " << ave_miss << ", " << ave_time << std::endl;

            ave_hits = ILS.hits/NUM_PARTITIONS; ave_miss = ILS.miss/NUM_PARTITIONS; ave_time = ILS.time/NUM_PARTITIONS;
            ILS.file << "promedio, "  << ave_hits << ", " << ave_miss << ", " << ave_time << std::endl;

            ave_hits = ILS_r.hits/NUM_PARTITIONS; ave_miss = ILS_r.miss/NUM_PARTITIONS; ave_time = ILS_r.time/NUM_PARTITIONS;
            ILS_r.file << "promedio, "  << ave_hits << ", " << ave_miss << ", " << ave_time << std::endl;

            ave_hits = SA.hits/NUM_PARTITIONS; ave_miss = SA.miss/NUM_PARTITIONS; ave_time = SA.time/NUM_PARTITIONS;
            SA.file << "promedio, "  << ave_hits << ", " << ave_miss << ", " << ave_time << std::endl;

            ave_hits = SA_r.hits/NUM_PARTITIONS; ave_miss = SA_r.miss/NUM_PARTITIONS; ave_time = SA_r.time/NUM_PARTITIONS;
            SA_r.file << "promedio, "  << ave_hits << ", " << ave_miss << ", " << ave_time << std::endl;

            // Close all files
            noW.file.close();
            rel.file.close();
            ILS.file.close();
            ILS_r.file.close();
            SA.file.close();
            SA_r.file.close();
        }
    }
    else {
        for (int name = 0; name < NUM_DATASETS; ++name) {

            Statistics stats(argv[1], name);

            // Read dataset file
            std::string dsFile = "datasets/" + dataNames[name] + "/" + dataNames[name] + ".data";
            DataSet dataset = readFile(dsFile.c_str());

            for (int i = 1; i < NUM_PARTITIONS + 1; ++i) {
                std::pair<DataSet, DataSet> ds = dataset.makePartition(i*10, 0.6);

                if (strcmp(argv[1], "no_weights") == 0) {
                    statisticsNoWeight (stats, name, i, ds);
                }
                else if (strcmp(argv[1], "relief") == 0) {
                    APC_Instance reliefV = statisticsRelief (stats, name, i, ds);
                }
                else if (strcmp(argv[1], "ILS_random") == 0) {
                    statisticsILSRand (stats, name, i, ds);
                }
                else if (strcmp(argv[1], "ILS_relief") == 0) {
                    std::vector<double> weights = relief(ds.first);
                    statisticsILSRelief (stats, name, i, ds, weights);
                }
                else if (strcmp(argv[1], "SA_random") == 0) {
                    statisticsSARand (stats, name, i, ds);
                }
                else if (strcmp(argv[1], "SA_relief") == 0) {
                    std::vector<double> weights = relief(ds.first);
                    statisticsSARelief (stats, name, i, ds, weights);
                }
            }

            // Average
            double ave_hits, ave_miss, ave_time;

            ave_hits = stats.hits/NUM_PARTITIONS; ave_miss = stats.miss/NUM_PARTITIONS; ave_time = stats.time/NUM_PARTITIONS;
            stats.file << "promedio, "  << ave_hits << ", " << ave_miss << ", " << ave_time << std::endl;
            stats.file.close();
        }
    }
    return 0;
}
