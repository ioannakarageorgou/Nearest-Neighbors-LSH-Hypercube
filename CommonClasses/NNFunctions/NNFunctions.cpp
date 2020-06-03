#include <chrono>
#include <iostream>
#include <cfloat>
#include "NNFunctions.h"
#include "../Manhattan.h"
#include "../DTW.h"


void exactNN(std::vector<DataVector> &queryDataset, std::vector<DataVector> &vectorDataset,
             std::vector<NNResult *> &exactNNResults, int radius) {

    double tempDistance, minDistance;

    for (auto &query : queryDataset) {

        std::pair<DataVector *, double> minNeighbour;
        minDistance = DBL_MAX;
        auto *result = new NNResult();
        auto start = std::chrono::system_clock::now();

        for (auto &vector : vectorDataset) {
            tempDistance = ManhattanDistance(query.getComponents(), vector.getComponents());


            if (tempDistance < radius && radius != 0) {
                result->addClosestNeighbour(vector.getId(), tempDistance);
            }

            if (tempDistance < minDistance && radius == 0) {
                minNeighbour.first = &vector;
                minNeighbour.second = tempDistance;
                minDistance = tempDistance;
            }
        }

        auto end = std::chrono::system_clock::now();

        result->setExecutionTime(end - start);
        result->setQueryId(query.getId());

        if (radius == 0)
            result->addClosestNeighbour((*minNeighbour.first).getId(), minNeighbour.second);


        exactNNResults.push_back(result);
    }
}