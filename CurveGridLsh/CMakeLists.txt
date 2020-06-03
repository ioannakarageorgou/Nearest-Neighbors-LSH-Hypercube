cmake_minimum_required(VERSION 3.15)
project(CurveGridLsh)

set(GCC_COVERAGE_COMPILE_FLAGS "-Ofast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

set(CMAKE_CXX_STANDARD 14)

add_executable(CurveGridLsh main.cpp ../CommonClasses/CurveToVector/CurveToVector.cpp ../CommonClasses/CurveToVector/CurveToVector.h ../CommonClasses/Grid/Grid.cpp ../CommonClasses/Grid/Grid.h ../CommonClasses/Curve/Curve.cpp ../CommonClasses/Curve/Curve.h ../CommonClasses/CurveGridInit/init.cpp ../CommonClasses/Utils.h ../CommonClasses/CurveGridInit/init.h
                ../CommonClasses/FileUtils/FileUtils.cpp ../CommonClasses/DataVector/DataVector.cpp ../CommonClasses/Euclidean.h
                ../CommonClasses/HashFunctions/hManhattan.cpp ../CommonClasses/HashFunctions/hashFunctionManhattan.cpp ../CommonClasses/NNResult/NNResult.cpp ../CommonClasses/NNFunctions/NNFunctions.cpp ../CommonClasses/Point/Point.h ../CommonClasses/CurveEntry/CurveEntry.cpp ../CommonClasses/CurveEntry/CurveEntry.h)