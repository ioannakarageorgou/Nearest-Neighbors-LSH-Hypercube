#include <iostream>
#include <vector>
#include <cstring>
#include "../CommonClasses/CurveGridInit/init.h"
#include "../CommonClasses/FileUtils/FileUtils.h"
#include "../lsh/lshManhattan.h"
#include "../CommonClasses/DTW.h"
#include "../CommonClasses/NNFunctions/NNFunctions.h"
#include "../CommonClasses/CurveEntry/CurveEntry.h"

//./CurveGridLsh -d ../../Dataset/Trajectory/trajectories_dataset -q ../../Dataset/Trajectory/query_trajectories -k_vec 4 -L_grid 1 -o output.txt

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
        if (strcmp(argv[i], "-k_vec") == 0) {
            k = atoi(argv[i + 1]);
        }
        if (strcmp(argv[i], "-L_grid") == 0) {
            L = atoi(argv[i + 1]);
        }
    }

}


int main(int argc, char **argv) {

    int L = 5, k = 4;
    //Read curves and constuct the LSH for Curves
    std::string inputFile, queryFile, outputFile;
    readArguments(argc, argv, k, L, inputFile, queryFile, outputFile);

    //std::string inputFile = "../../Dataset/Trajectory/trajectories_dataset";
    std::vector<Curve> curveDataset;
    int maxCurvePoints = readCurveDataset(inputFile, curveDataset);

    CurveToVector **LSHDataset = init(L, curveDataset);


    // std::string queryFile = "../../Dataset/Trajectory/query_trajectories";
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

    lshManhattan<CurveEntry> *apprModel = new lshManhattan<CurveEntry>(1, k, 14);        //approximate NN
    std::vector<CurveEntry> curveEntries;


    for (auto c_it = curveDataset.begin(); c_it != curveDataset.end(); c_it++) {
        c_it->addPadToGridVector(maxCurvePoints, 10000.0f);
    }

    for (auto c_it = curveDataset.begin(); c_it != curveDataset.end(); c_it++) {
        std::vector<Point> *points = &c_it->getComponents();
        std::vector<DataVector> *vectors = &c_it->myContents();

        for (auto v_it = vectors->begin(); v_it != vectors->end(); v_it++) {
            curveEntries.emplace_back(*points, &v_it->myContents());
        }
    }

    //std::cout << "δ = " << calculateD(curveEntries)<<std::endl;

    apprModel->fillStructures(curveEntries);

    std::vector<CurveEntry> queryEntries;

    for (auto c_it = queryDataset.begin(); c_it != queryDataset.end(); c_it++) {
        c_it->addPadToGridVector(maxCurvePoints, 10000.0f);
    }

    for (auto c_it = queryDataset.begin(); c_it != queryDataset.end(); c_it++) {
        std::vector<Point> *points = &c_it->getComponents();
        std::vector<DataVector> *vectors = &c_it->myContents();

        for (auto v_it = vectors->begin(); v_it != vectors->end(); v_it++) {
            queryEntries.emplace_back(*points, &v_it->myContents());
        }
    }

    auto start1 = std::chrono::system_clock::now();
    apprModel->approximateNN(queryEntries, curveEntries, approximateNNResults, 0, 500, DTW);
    auto end1 = std::chrono::system_clock::now();

    std::chrono::duration<double> executionTime1 = end1 - start1;

    std::cout << "Approx execution time:" << executionTime1.count() << std::endl;
    std::cout << "Average Approx execution time:" << executionTime1.count() / queryDataset.size() << std::endl;

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