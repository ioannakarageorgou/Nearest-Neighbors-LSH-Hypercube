
#ifndef HYPERCUBE_HYPERCUBEMANHATTAN_H
#define HYPERCUBE_HYPERCUBEMANHATTAN_H

#include <list>
#include <cfloat>
#include <bitset>
#include "../CommonClasses/DataVector/DataVector.h"
#include "../CommonClasses/HashFunctions/hashFunctionHyperCube.h"
#include "../CommonClasses/NNResult/NNResult.h"

template<typename S>
class hypercubeManhattan {
private:
    std::vector<std::list<S>> cube;
    hashFunctionHyperCube *hashFunctions;
    int tableSize;
    int n;
    int k;
    int dimension;
    int w;
    int M;
    int probes;
public:
    hypercubeManhattan(int k, int w, int M, int probes) : k(k), w(w), M(M), probes(probes) {

    }

    ~hypercubeManhattan() {
        delete this->hashFunctions;
    }

    void fillStructures(std::vector<S> &points, int _tableSize) {

        int i, pos;
        typename  std::vector<S>::iterator iterPoints = points.begin();

        this->n = points.size();
        this->tableSize = _tableSize;
        this->cube.reserve(this->tableSize);
        this->dimension = points[0].getDimension();

        for (i = 0; i < this->tableSize; i++) {
            this->cube.push_back(std::list<S>());
        }

        //set hash function
        hashFunctionHyperCube *newFunction = nullptr;
        newFunction = new hashFunctionHyperCube(this->dimension, this->k, this->w);
        if (newFunction == NULL) {
            std::cout << "Failed to allocate hManhattan function" << std::endl;
            return;
        }
        this->hashFunctions = newFunction;

        //set cube
        for (iterPoints = points.begin(); iterPoints != points.end(); iterPoints++) {
            pos = this->hashFunctions->hash(iterPoints->myContents());       //find position in cube
            this->cube[pos].push_back(*iterPoints);
        }

        std::cout << "Structures fit ok" << std::endl;
    }

    int entriesPerBucket() {

        int i;
        for (i = 0; i < this->k; i++) {
            std::cout << cube[i].size() << std::endl;
        }
    }

    template<typename T>
    void approximateNN(std::vector<S> &queryDataset, std::vector<S> &vectorDataset,
                       std::vector<NNResult *> &approximateNNResults, int radius, T MetricFunction) {


        int currentM, currentProbes, valueG, tempValueG;
        double tempDistance, minDistance;
        std::string valueGBinary;

        for (auto &query: queryDataset) {
            auto *result = new NNResult();
            auto start = std::chrono::system_clock::now();
            std::pair<S *, double> minNeighbour;
            minDistance = DBL_MAX;
            currentProbes = 0;

            valueG = hashFunctions->hash(query.myContents());
            tempValueG = valueG;
            //TODO to bitset prepei na einai megethous d', tipika mpainei tora 10
            valueGBinary = std::bitset<10>(valueG).to_string();

            while (currentProbes < this->probes && currentProbes < valueGBinary.size()) {
                valueGBinary = std::bitset<10>(valueG).to_string();
                std::list<S> *tempList = &cube.at(tempValueG);
                currentM = 0;

                while (currentM < tempList->size() && currentM < this->M) {
                    auto it = std::next(tempList->begin(), currentM);


                    tempDistance = MetricFunction(query.getComponents(), (*it).getComponents());

                    if (tempDistance < radius && radius != 0) {
                        result->addClosestNeighbour((*it).getId(), tempDistance);
                    }

                    if (tempDistance < minDistance && radius == 0) {
                        minNeighbour.first = &(*it);
                        minNeighbour.second = tempDistance;
                        minDistance = tempDistance;
                    }

                    currentM++;
                }

                if (valueGBinary[valueGBinary.size() - 1 - currentProbes] == '0')
                    valueGBinary[valueGBinary.size() - 1 - currentProbes] = '1';
                else
                    valueGBinary[valueGBinary.size() - 1 - currentProbes] = '0';

                tempValueG = std::stoi(valueGBinary, nullptr, 2);

                currentProbes++;
            }

            auto end = std::chrono::system_clock::now();

            result->setExecutionTime(end - start);
            result->setQueryId(query.getId());

            if (radius == 0)
                result->addClosestNeighbour((*minNeighbour.first).getId(), minNeighbour.second);

            approximateNNResults.push_back(result);
        }


    }
};


#endif //HYPERCUBE_HYPERCUBEMANHATTAN_H
