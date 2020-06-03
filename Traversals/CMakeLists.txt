cmake_minimum_required(VERSION 3.15)
project(Traversals)

set(CMAKE_CXX_STANDARD 14)

add_executable(Traversals main.cpp Position.h Traversals.cpp Traversals.h ../CommonClasses/FileUtils/FileUtils.cpp
                                       ../CommonClasses/DataVector/DataVector.cpp ../CommonClasses/NNResult/NNResult.cpp
                                        ../CommonClasses/Curve/Curve.cpp)