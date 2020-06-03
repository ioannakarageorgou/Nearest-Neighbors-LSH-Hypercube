#include <iostream>
#include "hashFunctionHyperCube.h"

hashFunctionHyperCube::hashFunctionHyperCube(int dimension, int k, int w) : k(k), w(w) {
    int i, j;

    this->H.reserve(k);

    //pick k hash functions
    hManhattan *newFunction;
    for (i = 0; i < this->k; i++) {
        newFunction = new hManhattan(w, k, dimension);
        if (newFunction == NULL) {
            std::cout << "Failed to allocate hManhattan function" << std::endl;
            break;
        }

        for (j = 0; j < i; j++) {
            if (this->H[j]->compare(*newFunction) == 0) {
                //found same sub hash function
                delete newFunction;
                break;
            }
        }
        if (j == i)
            this->H[i] = newFunction;
    }

    //fix maps distance
    for (i = 0; i < this->k; i++) {
        this->dist.push_back(std::uniform_int_distribution<int>(0, 1));
        this->hMaps.push_back(std::map<int, int>());
    }
}

hashFunctionHyperCube::~hashFunctionHyperCube() {

    for (int i = 0; i < this->k; i++) {
        delete this->H[i];
    }
}

// G(p) = f1(h1(p)).f2(h2(p))...fk(hk(p))
// Fi(p) maps hi(p) function to 0 or 1 (coin)
int hashFunctionHyperCube::hash(DataVector &x) {

    int i, currentF, currentH;
    std::map<int, int>::iterator iterator;
    std::string resultStr;

    //calculate h1(p)...hk(p)
    for (i = 0; i < this->k; i++) {

        currentH = this->H[i]->hash(x);

        iterator = this->hMaps[i].find(currentH);       //find if h(i) exists in map
        if (iterator != this->hMaps[i].end()) {    //exists in map
            currentF = iterator->second;
        } else {
            currentF = this->dist[i](this->generator);  //map current h(i)
            this->hMaps[i].insert(std::pair<int, int>(currentH, currentF));
        }

        resultStr += std::to_string(currentF);       //calculate g(p)
    }
    return std::stoi(resultStr, nullptr, 2);
}