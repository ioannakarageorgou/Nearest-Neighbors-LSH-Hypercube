
#ifndef EMIRIS1_HASHFUNCTIONMANHATTAN_H
#define EMIRIS1_HASHFUNCTIONMANHATTAN_H

#include "../DataVector/DataVector.h"
#include "hManhattan.h"

//g(x) = [h1(x),h2(x), ... ,hk(x)]
class hashFunctionManhattan {
private:
    std::vector<hManhattan *> H;     //H contains sub-hash functions
    int k;
    int w;
    int tableSize;
    std::vector<int> R; // Random ri values - standard

public:
    hashFunctionManhattan(int dimension, int k, int w, int tableSize);

    ~hashFunctionManhattan();

    unsigned int hash(DataVector &x);

    unsigned int concatenateH(std::vector<unsigned int> vect, int mod);

    int compare(hashFunctionManhattan &x);
};


#endif //EMIRIS1_HASHFUNCTIONMANHATTAN_H
