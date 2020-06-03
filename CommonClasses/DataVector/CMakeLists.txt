project(DataVector)

set(HEADER_FILES DataVector.h)

set(SOURCE_FILES DataVector.cpp)

add_library(DataVector STATIC ${SOURCE_FILES} ${HEADER_FILES})