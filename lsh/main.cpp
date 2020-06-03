#include <iostream>
#include <cstring>
#include <cmath>
#include "../CommonClasses/DataVector/DataVector.h"
#include "../CommonClasses/FileUtils/FileUtils.h"
#include "../CommonClasses/NNResult/NNResult.h"
#include "../CommonClasses/NNFunctions/NNFunctions.h"
#include "lshManhattan.h"
#include "../CommonClasses/Manhattan.h"

//./lsh -d ../../Dataset/Vector/input_small_id -q ../../Dataset/Vector/query_small_id -k 4 -L 5 -o output.txt

void readArguments(int argc, char **argv, int &k, int &L, std::string &inputFile, std::string &queryFile,
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
        if (strcmp(argv[i], "-L") == 0) {
            L = atoi(argv[i + 1]);
        }
    }

}

int main(int argc, char **argv) {
    std::vector<DataVector> vectorDataset;
    std::vector<DataVector> queryDataset;
    std::vector<NNResult *> exactNNResults;
    std::vector<NNResult *> approximateNNResults;

    // read user input
    int k = 4, L = 5;        //default values
    std::string inputFile, queryFile, outputFile;


    readArguments(argc, argv, k, L, inputFile, queryFile, outputFile);
    readVectorDataset(inputFile, vectorDataset);        //../../Dataset/Vector/input_small_id
    readVectorDataset(queryFile, queryDataset);         //../../Dataset/Vector/query_small_id

    auto start = std::chrono::system_clock::now();
    exactNN(queryDataset, vectorDataset, exactNNResults, 0);
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> executionTime = end - start;

    std::cout << "Exact execution time:" << executionTime.count() << std::endl;
    std::cout << "Average Exact execution time:" << executionTime.count()/queryDataset.size() << std::endl;

   // std::cout<<"W = "<< calculateW(exactNNResults)<<std::endl;


    lshManhattan<DataVector> *apprModel = new lshManhattan<DataVector>(L, k, 5467);        //approximate NN
    apprModel->fillStructures(vectorDataset);

    auto start1 = std::chrono::system_clock::now();
    apprModel->approximateNN(queryDataset, vectorDataset, approximateNNResults, 0,500, ManhattanDistance);
    auto end1 = std::chrono::system_clock::now();

    std::chrono::duration<double> executionTime1 = end1 - start1;

    std::cout<<"Approx execution time:" << executionTime1.count()<< std::endl;
    std::cout<<"Average Approx execution time:" << executionTime1.count() / queryDataset.size() << std::endl;


    exportDatavectorResults(outputFile, exactNNResults, approximateNNResults, 0);

    for (int i = 0; i < exactNNResults.size() && i < approximateNNResults.size(); i++) {
        delete exactNNResults.at(i);
        delete approximateNNResults.at(i);
    }

    delete apprModel;
    return 0;
}