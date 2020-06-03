#include <chrono>

#include "NNResult.h"
#include "../FileUtils/FileUtils.h"

std::string  NNResult::getQueryId() const {
    return queryId;
}

std::map<std::string , double> NNResult::getClosestNeighbour() const {
    return closestNeighbour;
}

std::chrono::duration<double> NNResult::getExecutionTime() const {
    return executionTime;
}


NNResult::NNResult() = default;

void NNResult::addClosestNeighbour(std::string  id, double distance) {
    closestNeighbour.insert({id,distance});
}

void NNResult::setQueryId(std::string  id) {
    queryId = id;
}

void NNResult::setExecutionTime(std::chrono::duration<double> _executionTime) {
    executionTime = _executionTime;
}










