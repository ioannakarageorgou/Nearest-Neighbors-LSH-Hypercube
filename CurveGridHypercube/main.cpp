#include <iostream>
#include <vector>
#include <cstring>

#include "../CommonClasses/CurveGridInit/init.h"
#include "../CommonClasses/FileUtils/FileUtils.h"
#include "../CommonClasses/DTW.h"
#include "../CommonClasses/NNFunctions/NNFunctions.h"
#include "../CommonClasses/CurveEntry/CurveEntry.h"
#include "../Hypercube/hypercubeManhattan.h"

//./CurveGridHypercube -d ../../Dataset/Trajectory/trajectories_dataset -q ../../Dataset/Trajectory/query_trajectories -k_hypercube 8 -L_grid 1 -M 200 -probes 300 -o output.txt

void readArguments(int argc, char **argv, int &k, int &L, int &M, int &probes, std::string &inputFile,
                   std::string &queryFile,
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
        if (strcmp(argv[i], "-k_hypercube") == 0) {
            k = atoi(argv[i + 1]);
        }
        if (strcmp(argv[i], "-M") == 0) {
            M = atoi(argv[i + 1]);
        }
        if (strcmp(argv[i], "-probes") == 0) {
            probes = atoi(argv[i + 1]);
        }
        if (strcmp(argv[i], "-L_grid") == 0) {
            probes = atoi(argv[i + 1]);
        }
    }
}

int main(int argc, char **argv) {
    int k = 4, L = 5, M = 20, probes = 30;
    //Read curves and constuct the LSH for Curves
    std::string inputFile, queryFile, outputFile;
    readArguments(argc, argv, k, L, M, probes, inputFile, queryFile, outputFile);

    //std::string inputFile = "../../Dataset/Trajectory/trajectories_dataset";
    std::vector<Curve> curveDataset;
    int maxCurvePoints = readCurveDataset(inputFile, curveDataset);
    CurveToVector **LSHDataset = init(L, curveDataset);


    //std::string queryFile = "../../Dataset/Trajectory/query_trajectories";
    std::vector<Curve> queryDataset;
    int maxQueryPoints = readCurveDataset(queryFile, queryDataset);
    CurveToVector **LSHQuery = init(L, queryDataset);


    std::vector<NNResult *> exactNNResults;
    auto start = std::chrono::system_clock::now();
    exactNNcurves(queryDataset, curveDataset, exactNNResults, DTW);
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> executionTime = end - start;

    std::cout << "Exact execution time:" << executionTime.count() << std::endl;
    std::cout << "Average Exact execution time:" << executionTime.count() / queryDataset.size() << std::endl;

    //std::cout << "W = " << calculateW(exactNNResults) << std::endl;


    std::vector<NNResult *> approximateNNResults;

    hypercubeManhattan<CurveEntry> *apprModel = new hypercubeManhattan<CurveEntry>(k, 14, M,
                                                                                   probes);        //approximate NN
    std::vector<CurveEntry> curveEntries;

    for (auto c_it = curveDataset.begin(); c_it != curveDataset.end(); c_it++) {
        c_it->addPadToGridVector(maxCurvePoints, -2000.0f);
    }

    for (auto c_it = curveDataset.begin(); c_it != curveDataset.end(); c_it++) {
        std::vector<Point> *points = &c_it->getComponents();
        std::vector<DataVector> *vectors = &c_it->myContents();

        for (auto v_it = vectors->begin(); v_it != vectors->end(); v_it++) {
            curveEntries.emplace_back(*points, &v_it->myContents());
        }
    }

    apprModel->fillStructures(curveEntries, 800);

    std::vector<CurveEntry> queryEntries;

    for (auto c_it = queryDataset.begin(); c_it != queryDataset.end(); c_it++) {
        c_it->addPadToGridVector(maxCurvePoints, -2000.0f);
    }

    for (auto c_it = queryDataset.begin(); c_it != queryDataset.end(); c_it++) {
        std::vector<Point> *points = &c_it->getComponents();
        std::vector<DataVector> *vectors = &c_it->myContents();

        for (auto v_it = vectors->begin(); v_it != vectors->end(); v_it++) {
            queryEntries.emplace_back(*points, &v_it->myContents());
        }
    }

    auto start1 = std::chrono::system_clock::now();
    apprModel->approximateNN(queryEntries, curveEntries, approximateNNResults, 0, DTW);
    auto end1 = std::chrono::system_clock::now();

    std::chrono::duration<double> executionTime1 = end1 - start1;

    std::cout << "Approx execution time:" << executionTime1.count() << std::endl;
    std::cout << "Average Approx execution time:" << executionTime1.count() / queryDataset.size() << std::endl;

    std::cout << approximateNNResults.size() << std::endl;


    exportCurveResults(outputFile, exactNNResults, approximateNNResults, L);


    for (int i = 0; i < exactNNResults.size() && i < approximateNNResults.size(); i++) {
        delete exactNNResults.at(i);
        delete approximateNNResults.at(i);
    }

    for (int i = 0; i < curveDataset.size(); i++) {
        delete LSHDataset[i];
    }
    delete LSHDataset;

    for (int i = 0; i < curveDataset.size(); i++) {
        delete LSHQuery[i];
    }
    delete LSHQuery;

    delete apprModel;

    return 0;
}