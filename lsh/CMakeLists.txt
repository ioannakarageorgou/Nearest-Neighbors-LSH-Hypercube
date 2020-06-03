cmake_minimum_required(VERSION 3.14)
project(emiris1)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp ../CommonClasses/FileUtils/FileUtils.cpp ../CommonClasses/NNFunctions/NNFunctions.cpp
                            ../CommonClasses/NNResult/NNResult.cpp  ../CommonClasses/HashFunctions/hManhattan.cpp
                            ../CommonClasses/HashFunctions/hashFunctionManhattan.cpp lshManhattan.h
                            ../CommonClasses/DataVector/DataVector.cpp ../CommonClasses/Manhattan.h ../CommonClasses/Curve/Curve.cpp)
add_executable(emiris1 ${SOURCE_FILES})



