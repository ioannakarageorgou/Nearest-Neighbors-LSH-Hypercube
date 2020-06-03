cmake_minimum_required(VERSION 3.15)
project(CurveGridHypercube)

set(GCC_COVERAGE_COMPILE_FLAGS "-Ofast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

set(CMAKE_CXX_STANDARD 14)

add_executable(CurveGridHypercube main.cpp ../CommonClasses/Curve/Curve.cpp ../CommonClasses/Curve/Curve.h
                                            ../CommonClasses/DataVector/DataVector.cpp ../CommonClasses/DataVector/DataVector.h
                                            ../CommonClasses/CurveEntry/CurveEntry.cpp ../CommonClasses/CurveEntry/CurveEntry.h
                                            ../Hypercube/hypercubeManhattan.h
                                            ../CommonClasses/CurveGridInit/init.cpp ../CommonClasses/CurveGridInit/init.h
                                            ../CommonClasses/FileUtils/FileUtils.cpp ../CommonClasses/FileUtils/FileUtils.h
                                            ../CommonClasses/CurveToVector/CurveToVector.cpp ../CommonClasses/CurveToVector/CurveToVector.h
                                            ../CommonClasses/Grid/Grid.cpp ../CommonClasses/Grid/Grid.h
                                            ../CommonClasses/HashFunctions/hashFunctionHyperCube.cpp  ../CommonClasses/HashFunctions/hashFunctionHyperCube.h
                                            ../CommonClasses/HashFunctions/hManhattan.cpp ../CommonClasses/HashFunctions/hManhattan.h
                                            ../CommonClasses/NNResult/NNResult.cpp)