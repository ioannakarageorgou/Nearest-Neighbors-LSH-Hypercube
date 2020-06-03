cmake_minimum_required(VERSION 3.14)
project(hypercube)

set(CMAKE_CXX_STANDARD 14)

#add_executable(hypercube main.cpp hypercubeManhattan.cpp hypercubeManhattan.h hashFunction.cpp hashFunction.h)
set(SOURCE_FILES main.cpp ../CommonClasses/HashFunctions/hManhattan.cpp  ../CommonClasses/HashFunctions/hashFunctionHyperCube.cpp ../CommonClasses/NNResult/NNResult.cpp
         ../CommonClasses/DataVector/DataVector.cpp ../CommonClasses/FileUtils/FileUtils.h ../CommonClasses/FileUtils/FileUtils.cpp
        ../CommonClasses/NNFunctions/NNFunctions.h ../CommonClasses/NNFunctions/NNFunctions.cpp ../CommonClasses/Curve/Curve.cpp)

add_executable(hypercube ${SOURCE_FILES})
