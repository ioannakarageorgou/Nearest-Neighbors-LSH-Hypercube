#include <iostream>
#include <cstring>
#include "../CommonClasses/DataVector/DataVector.h"
#include "../CommonClasses/FileUtils/FileUtils.h"
#include "../CommonClasses/NNFunctions/NNFunctions.h"
#include "../CommonClasses/NNResult/NNResult.h"
#include "hypercubeManhattan.h"
#include "../CommonClasses/Manhattan.h"

//./hypercube -d ../../Dataset/Vector/input_small_id -q ../../Dataset/Vector/query_small_id -k 4 -M 100 -probes 30 -o output.txt

void readArguments(int argc, char **argv, int &k, int &M, int &probes, std::string &inputFile, std::string &queryFile,
                   std::string &outputFile) {

    if (argc == 1) {
        std::cout << "No arguments given" << std::endl;
        exit(0);
    }

    for (int i = 0; i < argc; i++) {

        if (strcmp(argv[i], "-d") == 0) {
            inputFile = argv[i + 1];
        }
        if (strcmp(argv[i], "-q") == 0) {
            queryFile = argv[i + 1];
        }
        if (strcmp(argv[i], "-o") == 0) {
            outputFile = argv[i + 1];
        }
        if (strcmp(argv[i], "-k") == 0) {
            k = atoi(argv[i + 1]);
        }
        if (strcmp(argv[i], "-M") == 0) {
            M = atoi(argv[i + 1]);
        }
        if (strcmp(argv[i], "-probes") == 0) {
            probes = atoi(argv[i + 1]);
        }
    }
}

int main(int argc, char **argv) {
    std::vector<DataVector> vectorDataset;
    std::vector<DataVector> queryDataset;
    std::vector<NNResult *> exactNNResults;
    std::vector<NNResult *> approximateNNResults;

    // read user input
    int k = 4, M = 10, probes = 20;        //default values
    std::string inputFile, queryFile, outputFile;

    readArguments(argc, argv, k, M, probes, inputFile, queryFile, outputFile);
    readVectorDataset(inputFile, vectorDataset);
    readVectorDataset(queryFile, queryDataset);

    auto start = std::chrono::system_clock::now();
    exactNN(queryDataset, vectorDataset, exactNNResults, 0);
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> executionTime = end - start;

    std::cout << "Exact execution time:" << executionTime.count() << std::endl;
    std::cout << "Average Exact execution time:" << executionTime.count() / queryDataset.size() << std::endl;

    //std::cout << "W = " << calculateW(exactNNResults) << std::endl;

    hypercubeManhattan<DataVector> *apprModel = new hypercubeManhattan<DataVector>(k, 5467, M,
                                                                                   probes);        //approximate NN
    apprModel->fillStructures(vectorDataset, 800);

    auto start1 = std::chrono::system_clock::now();
    apprModel->approximateNN(queryDataset, vectorDataset, approximateNNResults, 0, ManhattanDistance);
    auto end1 = std::chrono::system_clock::now();

    std::chrono::duration<double> executionTime1 = end1 - start1;

    std::cout << "Approx execution time:" << executionTime1.count() << std::endl;
    std::cout << "Average Approx execution time:" << executionTime1.count() / queryDataset.size() << std::endl;


    exportDatavectorResults(outputFile, exactNNResults, approximateNNResults, 0);

    for (int i = 0; i < exactNNResults.size() && i < approximateNNResults.size(); i++) {
        delete exactNNResults.at(i);
        delete approximateNNResults.at(i);
    }

    delete apprModel;
    return 0;
}