/**
 *  Universidad Simon Bolivar
 *  CI-5652 - Diseño de Algoritmos II
 *  @author : Erick Silva
 *  @author : Stefani Castellanos 11-11394
 *
 *  Statistics for APC problem
 */

#include <string>
#include <ctime>
#include "dataset.hpp"
#include "relief.hpp"
#include "NearestNeighbor.cpp"
#include "ILS.hpp"

int NUM_PARTITIONS = 4;
int NUM_DATASETS = 4;
std::vector<string> dataNames = {"iris", "sonar", "wdbc", "spambase" };
std::vector<int> maxIterations = {2, 2, 2, 2 };
std::vector<int> neighborsPerGen = {3, 3, 3, 3 };

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
int main(int argc, char const *argv[]) {

    time_t timeStart;
    time_t timeEnd;
    double timeElapsed;
    double nHits, nError;
    string header = "Particion, Aciertos(\%) , Error(\%), Tiempo(sg) ";

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

            // Statistics without weights
            std::cout << "Executing K-NN algorithm without weights on " << dataNames[name] << " dataset (" << i << ")" << std::endl;
            timeStart = time(NULL);
            std::vector<double> weights1(ds.first.nFeatures, 1.0);
            nHits = NN1(ds.first, ds.second, weights1);
            timeEnd = time(NULL);
            timeElapsed = difftime(timeEnd, timeStart);
            nError = 100 - nHits;
            noW.update(nHits, nError, timeElapsed);
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
            rel.update(nHits, nError, timeElapsed);
            fileR << i << ", " << nHits << ", " << nError << ", " << timeElapsed << std::endl;
            std::cout << "done!" << std::endl;

            // Statistics ILS random
            std::cout << "Executing ILS (random) algorithm on " << dataNames[name] << " dataset (" << i << ")" << std::endl;
            timeStart = time(NULL);
            APC_Instance w(dataset.nFeatures);
            APC_Instance weights3 = ILS_convergence(w, ds.first, ds.second, maxIterations[name], neighborsPerGen[name], 2);
            nHits = weights3.evaluate(ds.first, ds.second);
            timeEnd = time(NULL);
            timeElapsed = difftime(timeEnd, timeStart);
            nError = 100 - nHits;
            ILS.update(nHits, nError, timeElapsed);
            fileILS << i << ", " << nHits << ", " << nError << ", " << timeElapsed << std::endl;
            std::cout << "done!" << std::endl;

            // Statistics ILS RELIEF
            std::cout << "Executing ILS (RELIEF) algorithm on " << dataNames[name] << " dataset (" << i << ")" << std::endl;
            timeStart = time(NULL);
            APC_Instance w2(weights2);
            APC_Instance weights4 = ILS_convergence(w2, ds.first, ds.second, maxIterations[name], neighborsPerGen[name], 2);
            nHits = weights4.evaluate(ds.first, ds.second);
            timeEnd = time(NULL);
            timeElapsed = difftime(timeEnd, timeStart);
            nError = 100 - nHits;
            ILS_r.update(nHits, nError, timeElapsed);
            fileILSR << i << ", " << nHits << ", " << nError << ", " << timeElapsed << std::endl;
            std::cout << "done!" << std::endl;
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
