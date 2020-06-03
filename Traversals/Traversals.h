#ifndef TRAVERSALS_TRAVERSALS_H
#define TRAVERSALS_TRAVERSALS_H

#include <vector>
#include "Position.h"

void findTraversals(double curveXSize, double curveYSize, double x, double y, bool yellowPath,
                    std::vector<std::vector<Position>> &traversals,
                    std::vector<Position> *inheritedPath);


#endif //TRAVERSALS_TRAVERSALS_H
