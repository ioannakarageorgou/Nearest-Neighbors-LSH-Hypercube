#ifndef PROJECT_EMIRIS_NNRESULT_H
#define PROJECT_EMIRIS_NNRESULT_H

#include <map>
#include "../DataVector/DataVector.h"
#include <chrono>


class NNResult {
private:
    std::string  queryId;
    std::map<std::string ,double> closestNeighbour;
    std::chrono::duration<double> executionTime;

public:

    NNResult();

    std::string  getQueryId() const;

    void setQueryId(std::string  id);

    std::map<std::string ,double> getClosestNeighbour() const;

    std::chrono::duration<double> getExecutionTime() const;

    void addClosestNeighbour(std::string  id, double distance);

    void setExecutionTime(std::chrono::duration<double> executionTime);


};

#endif //PROJECT_EMIRIS_NNRESULT_H
