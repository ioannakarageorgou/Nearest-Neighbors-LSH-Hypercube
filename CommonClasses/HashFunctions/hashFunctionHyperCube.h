#ifndef HYPERCUBE_HASHFUNCTIONHYPERCUBE_H
#define HYPERCUBE_HASHFUNCTIONHYPERCUBE_H

#include <map>
#include <random>
#include "hManhattan.h"

//G(p) = f1(h1(p)).f2(h2(p))...fk(hk(p))
class hashFunctionHyperCube {
private:
    std::vector<hManhattan*> H;
    std::vector<std::map<int,int>> hMaps;      // Keep in map f unique values
    int k;
    int w;
    std::vector<std::uniform_int_distribution<int> > dist; // DIstributions for fi
    std::default_random_engine generator;
public:
    hashFunctionHyperCube(int dimension, int k, int w);
    ~hashFunctionHyperCube();

    int hash(DataVector& x);
};


#endif //HYPERCUBE_HASHFUNCTIONHYPERCUBE_H
