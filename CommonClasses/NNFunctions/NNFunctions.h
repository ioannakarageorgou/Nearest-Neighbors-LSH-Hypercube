#ifndef PROJECT_EMIRIS_NNFUNCTIONS_H
#define PROJECT_EMIRIS_NNFUNCTIONS_H

#include <map>
#include <cfloat>
#include "../../CommonClasses/DataVector/DataVector.h"
#include "../../CommonClasses/NNResult/NNResult.h"
#include "../Curve/Curve.h"


template<typename T>
void exactNNcurves(std::vector<Curve> &queryDataset, std::vector<Curve> &curveDataset,
                   std::vector<NNResult *> &exactNNResults, T MetricFunction) {

    double minDistance, tempDistance;

    for (auto &query: queryDataset) {

        std::pair<Curve *, double> minNeighbour;
        minDistance = DBL_MAX;
        auto *result = new NNResult();
        auto start = std::chrono::system_clock::now();

        for (auto &curve : curveDataset) {

            tempDistance = MetricFunction(query.getComponents(), curve.getComponents());

            if (tempDistance < minDistance) {
                minNeighbour.first = &curve;
                minNeighbour.second = tempDistance;
                minDistance = tempDistance;
            }
        }
        auto end = std::chrono::system_clock::now();

        result->setExecutionTime(end - start);
        result->setQueryId(query.getID());
        result->addClosestNeighbour((*minNeighbour.first).getID(), minNeighbour.second);

        exactNNResults.push_back(result);
    }
}

void exactNN(std::vector<DataVector> &queryDataset, std::vector<DataVector> &vectorDataset,
             std::vector<NNResult *> &exactNNResults, int radius);


#endif //PROJECT_EMIRIS_NNFUNCTIONS_H
