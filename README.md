## Nearest and radius neighbors with LSH and hypercube for points and polygonal curves

##### LSH
*lshManhattan.h *

*struct Entries* : structure that contains the elements on which we implement LSH on. It	stores point for this exact implementation.

*class lshManhattan* : contains all the structures that we need to implement LSH. It contains a vector with hash tables,on which we store Entries.
There is also a vector of hash functions because each hash table has its own hash function.
Here are stored the variables L,k, number of items, dimension of each item and window size.

*FillStructures* : fixs hash table, members of lsh manhattan and add given points in the hash tables

*ApproximateNN* : finds the one approximate neighbor of an item and also the approximate neighbors within an r distance (if given the appropriate argument in radius).

##### Hypercube
*hypercubeManhattan.h*

*class hypercubeManhattan* : we represent cube as a vector of lists that contains DataVector, one hash function and information as table size, number of items, k, dimension , window size, max number of checked points(M), max number of checked vertices (probes).

*fillStructures* : fixs hash function, members of hypercube manhattan and add given points in the hash tables

*approximateNN* : finds the one approximate neighbor of an item and also the approximate neighbors within an r distance (if given the appropriate argument in radius).

##### Common Classes
*DataVector.h/.cpp*

*class DataVector* : contains the vector points , the id of vector and its dimension.

*hmanhattan.h/.cpp*

*hash* :  computes each h(i) = a{d-1} + m a{d-2} + ... + m^(d-1) a0, where a(i) = floor((x(i)-s(i))/W), M = 2^(32/k) and m = 2^32 – 5.

*HashFunctionManhattan.h/.cpp*

*hashFunctionManhattan* : picks k hash functions, one for each table.

*hash* : we provide 2 different types of  computing g. One is defined by concatenation and the other is random linear combination.

*getRandomUniform* : picks single-precision real t uniformly ∈R [0, w).

*NNFunctions.h/.cpp*

*exactNN* : finds the one real neighbor of an item using exhaustive search.
