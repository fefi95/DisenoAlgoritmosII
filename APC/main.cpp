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

int NUM_PARTITIONS = 4;
int NUM_DATASETS = 1;
std::vector<string> dataNames = {"iris", "sonar", "wdbc", "spambase" };
std::vector<int> maxIterations = {2, 2, 2, 2 };
std::vector<int> neighborsPerGen = {3, 3, 3, 3 };
time_t timeStart;
time_t timeEnd;
double timeElapsed;
double nHits, nError;
string header = "Particion, Aciertos(\%) , Error(\%), Tiempo(sg) ";

class Statistics {
public:
    double hits;
    double miss;
    double time;

    Statistics(){
        hits = 0;
        miss = 0;
        time = 0.0;
    };

    void update(double nHits, double nMiss, double timeE){
        hits += nHits;
        miss += nMiss;
        time += timeE;
    }
};

// Statistics without weights
void statisticsNoWeight(ofstream &file,
                        Statistics &stats,
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
    file << part << ", " << nHits << ", " << nError << ", " << timeElapsed << std::endl;
    std::cout << "done!" << std::endl;
}

// Statistics RELIEF
APC_Instance statisticsRelief(ofstream &file,
                              Statistics &stats,
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
    file << part << ", " << nHits << ", " << nError << ", " << timeElapsed << std::endl;
    std::cout << "done!" << std::endl;

    return weights;
}

// Statistics ILS random
void statisticsILSRand(ofstream &file,
                      Statistics &stats,
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
    file << part << ", " << nHits << ", " << nError << ", " << timeElapsed << std::endl;
    std::cout << "done!" << std::endl;

}

// Statistics ILS random
void statisticsILSRelief(ofstream &file,
                      Statistics &stats,
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
    file << part << ", " << nHits << ", " << nError << ", " << timeElapsed << std::endl;
    std::cout << "done!" << std::endl;

}

int main(int argc, char const *argv[]) {

    // Statistics
    for (int name = 0; name < NUM_DATASETS; ++name) {

        // File statistics for no weight
        ofstream fileNoW;
        fileNoW.open("../Paper/statistics/" + dataNames[name] + "/no_weights.csv");
        fileNoW << header << std::endl;
        Statistics noW;

        // File statistics for RELIEF
        ofstream fileR;
        fileR.open("../Paper/statistics/" + dataNames[name] + "/relief.csv");
        fileR << header << std::endl;
        Statistics rel;

        // File statistics for ILS random
        ofstream fileILS;
        fileILS.open("../Paper/statistics/" + dataNames[name] + "/ILS_random.csv");
        fileILS << header << std::endl;
        Statistics ILS;

        // File statistics for ILS RELIEF
        ofstream fileILSR;
        fileILSR.open("../Paper/statistics/" + dataNames[name] + "/ILS_relief.csv");
        fileILSR << header << std::endl;
        Statistics ILS_r;

        // Read dataset file
        std::string dsFile = "datasets/" + dataNames[name] + "/" + dataNames[name] + ".data";
        DataSet dataset = readFile(dsFile.c_str());

        for (int i = 1; i < NUM_PARTITIONS + 1; ++i) {
            std::pair<DataSet, DataSet> ds = dataset.makePartition(i*10, 0.6);

            if (strcmp(argv[1], "no_weights") == 0) {
                statisticsNoWeight (fileNoW, noW, name, i, ds);
            }
            else if (strcmp(argv[1], "relief") == 0) {
                APC_Instance reliefV = statisticsRelief (fileR, rel, name, i, ds);
            }
            else if (strcmp(argv[1], "ILS_random") == 0) {
                statisticsILSRand ( fileILS, ILS, name, i, ds);
            }
            else if (strcmp(argv[1], "ILS_relief") == 0) {
                  std::vector<double> weights = relief(ds.first);
                statisticsILSRelief ( fileILSR, ILS_r, name, i, ds, weights);
            }
            else if (strcmp(argv[1], "all") == 0) {
                statisticsNoWeight (fileNoW, noW, name, i, ds);
                APC_Instance reliefV = statisticsRelief (fileR, rel, name, i, ds);
                statisticsILSRand ( fileILS, ILS, name, i, ds);
                statisticsILSRelief ( fileILSR, ILS_r, name, i, ds, reliefV);
            }
        }

        // Average
        double ave_hits, ave_miss, ave_time;

        ave_hits = noW.hits/NUM_PARTITIONS; ave_miss = noW.miss/NUM_PARTITIONS; ave_time = noW.time/NUM_PARTITIONS;
        fileNoW << "promedio, "  << ave_hits << ", " << ave_miss << ", " << ave_time << std::endl;

        ave_hits = rel.hits/NUM_PARTITIONS; ave_miss = rel.miss/NUM_PARTITIONS; ave_time = rel.time/NUM_PARTITIONS;
        fileR << "promedio, "  << ave_hits << ", " << ave_miss << ", " << ave_time << std::endl;

        ave_hits = ILS.hits/NUM_PARTITIONS; ave_miss = ILS.miss/NUM_PARTITIONS; ave_time = ILS.time/NUM_PARTITIONS;
        fileILS << "promedio, "  << ave_hits << ", " << ave_miss << ", " << ave_time << std::endl;

        ave_hits = ILS_r.hits/NUM_PARTITIONS; ave_miss = ILS_r.miss/NUM_PARTITIONS; ave_time = ILS_r.time/NUM_PARTITIONS;
        fileILSR << "promedio, "  << ave_hits << ", " << ave_miss << ", " << ave_time << std::endl;

        // Close all files
        fileR.close();
        fileNoW.close();
        fileILS.close();
        fileILSR.close();
    }


    return 0;
}
