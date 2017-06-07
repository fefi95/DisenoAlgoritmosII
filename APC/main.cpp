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

int main(int argc, char const *argv[]) {
    DataSet dataset = readFile("datasets/iris/iris.data");
    relief(dataset);
    return 0;
}
