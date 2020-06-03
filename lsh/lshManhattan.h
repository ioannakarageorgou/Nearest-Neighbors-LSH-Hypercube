#ifndef EMIRIS1_LSHMANHATTAN_H
#define EMIRIS1_LSHMANHATTAN_H

#include "../CommonClasses/DataVector/DataVector.h"
#include "../CommonClasses/HashFunctions/hashFunctionManhattan.h"
#include "../CommonClasses/NNResult/NNResult.h"
#include "../CommonClasses/Euclidean.h"
#include <cfloat>
#include <list>

template<typename S>
struct Entries {
    S *point;
    int valueG;        // Value of g hash function

    void addPoint(std::vector<S> &points, int p) { this->point = &(points[p]); }

    void addG(int valueG) { this->valueG = valueG; }
};

template<typename S>
class lshManhattan {
private:
    //std::vector<DataVector> points;
    //TODO Map gia tables/hashFunction
    std::vector<std::vector<std::list<Entries<S>>>> tables;        // Each table is a hash table-vector of lists
    std::vector<hashFunctionManhattan *> hashFunctions;                 // Each table has one hash function
    int tableSize;
    int n; // Number of items
    int l; // Total tables, hash functions
    int k; // Number of sub hash functions
    int dimension;
    int w; // Window size
    int hasData;        //for checking reasons only- deallocation check
public:
    virtual ~lshManhattan() {

        if (this->hasData == 1) {         // 1->has data -- 0->no data
            for (int i = 0; i < this->l; i++) {
                delete this->hashFunctions[i];
            }
        }

    }

    lshManhattan(int l, int k, int w) : l(l), k(k), w(w) {

        this->hashFunctions.reserve(this->l);       //num of hash functions

        for (int i = 0; i < this->l; i++) {
            this->tables.push_back(std::vector<std::list<Entries<S>>>(this->l));
        }
    }

    void fillStructures(std::vector<S> &dataset) {

        typename std::vector<S>::iterator iterPoints = dataset.begin();
        typename std::list<Entries<S>>::iterator iterEntries;

        if (this->hasData == 1) {
            std::cout << "Method already used" << std::endl;
            return;
        }

        this->n = dataset.size();
        this->tableSize = this->n / 8;              //tableSize = n/8

        for (int i = 0; i < this->l; i++) {
            //each table contains lists of entries
            this->tables[i].reserve(this->tableSize);
            for (int j = 0; j < this->tableSize; j++) {
                this->tables[i].push_back(std::list<Entries<S>>());
            }
        }

        this->dimension = dataset[0].getDimension();             //all have same dimension

        //Set hash functions
        hashFunctionManhattan *newFunction = nullptr;
        int i, j, p;

        for (i = 0; i < this->l; i++) {

            newFunction = new hashFunctionManhattan(this->dimension, this->k, this->w, this->tableSize);
            if (newFunction == NULL) {
                std::cout << "Failed to allocate hashFunctionManhattan" << std::endl;
                break;
            }

            //find same hash functions and delete the one of them
            for (j = 0; j < i; j++) {
                if (this->hashFunctions[j]->compare(*newFunction) == 0) {
                    delete newFunction;
                    break;
                }
            }

            if (i == j) {                   //if function is new place it in the table
                this->hashFunctions.push_back(newFunction);
            }
        }

        //set hash tables

        int pos;

        //scan each table
        for (i = 0; i < this->l; i++) {
            //scan points
            for (p = 0; p < this->n; p++) {
                //find position in the hash table
                pos = this->hashFunctions[i]->hash(dataset[p].myContents());

                int newValueG = this->hashFunctions[i]->hash(dataset[p].myContents());

                //set new entry
                Entries<S> newEntry;
                newEntry.addPoint(dataset, p);
                newEntry.addG(newValueG);

                this->tables[i][pos].push_back(newEntry);       //add point
            }
        }


        this->hasData = 1;
        std::cout << "Structures fit ok" << std::endl;
    }

    int entriesPerBucket() {
        int i, p;

        //scan each table
        for (i = 0; i < this->l; i++) {
            for (p = 0; p < this->k; p++) {
                std::cout << tables[i][p].size() << std::endl;
            }
        }
    }

    template<typename T>
    void approximateNN(std::vector<S> &queryDataset, std::vector<S> &vectorDataset,
                       std::vector<NNResult *> &approximateNNResults, int radius,int threshold, T MetricFunction) {

        int index, valueG, counter, neighbourFound;
        double tempDistance, minDistance;
        for (auto &query: queryDataset) {
            index = 0;
            neighbourFound = 0;
            auto *result = new NNResult();
            std::pair<S *, double> minNeighbour;
            minDistance = DBL_MAX;

            auto start = std::chrono::system_clock::now();

            for (auto &table: tables) {

                counter = 0;

                valueG = hashFunctions.at(index)->hash(query.myContents());

                for (Entries<S> &entry:  table.at(valueG)) {
                    if (counter > threshold)
                        break;

                    if (valueG == entry.valueG) {

                        tempDistance = MetricFunction(query.getComponents(), (*entry.point).getComponents());

                        if (tempDistance < radius && radius != 0) {
                            result->addClosestNeighbour((*entry.point).getId(), tempDistance);
                        }

                        if (tempDistance < minDistance && radius == 0) {
                            minNeighbour.first = &(*entry.point);
                            minNeighbour.second = tempDistance;
                            minDistance = tempDistance;
                            neighbourFound = 1;
                        }

                        counter++;
                    }
                }
                index++;
            }

            auto end = std::chrono::system_clock::now();

            result->setExecutionTime(end - start);
            result->setQueryId(query.getId());

            if (radius == 0 && neighbourFound == 1)
                result->addClosestNeighbour((*minNeighbour.first).getId(), minNeighbour.second);

            if (neighbourFound == 1)
                approximateNNResults.push_back(result);
            else{
                result->addClosestNeighbour("NONE FOUND", 0.0f);
                approximateNNResults.push_back(result);
            }
        }
    }

};


#endif //EMIRIS1_LSHMANHATTAN_H
