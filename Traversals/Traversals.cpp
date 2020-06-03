#include "Traversals.h"


void findTraversals(double curveXSize, double curveYSize, double x, double y, bool yellowPath,
                    std::vector<std::vector<Position>> &traversals,
                    std::vector<Position> *inheritedPath) {

    inheritedPath->push_back(Position(x, y));

    if (x == curveXSize && y == curveYSize) {
        traversals.push_back(*inheritedPath);
        return;
    }

    if (x / curveXSize < y / curveYSize) {

        if (x / curveXSize >= (y - 1) / curveYSize)
            findTraversals(curveXSize, curveYSize, x + 1, y, false, traversals, inheritedPath);
        else
            findTraversals(curveXSize, curveYSize, x + 1, y, true, traversals, inheritedPath);


        if ((x + 1) / curveXSize > y / curveYSize && y != curveYSize) {
            auto *path = new std::vector<Position>;

            for (int i = 0; i < inheritedPath->size(); i++)
                path->push_back(inheritedPath->at(i));

            findTraversals(curveXSize, curveYSize, x + 1, y + 1, false, traversals, path);
        }

        if ((x + 1) / curveXSize == y / curveYSize && y != curveYSize) {
            auto *path = new std::vector<Position>;

            for (int i = 0; i < inheritedPath->size(); i++)
                path->push_back(inheritedPath->at(i));

            findTraversals(curveXSize, curveYSize, x + 1, y + 1, true, traversals, path);
        }

        if (y != curveYSize && yellowPath == false) {
            auto *path = new std::vector<Position>;

            for (int i = 0; i < inheritedPath->size(); i++)
                path->push_back(inheritedPath->at(i));

            findTraversals(curveXSize, curveYSize, x, y + 1, true, traversals, path);
        }

    } else if (x / curveXSize > y / curveYSize) {

        findTraversals(curveXSize, curveYSize, x, y + 1, false, traversals, inheritedPath);

        if (x / curveXSize < (y + 1) / curveYSize && x != curveXSize) {
            auto *path = new std::vector<Position>;

            for (int i = 0; i < inheritedPath->size(); i++)
                path->push_back(inheritedPath->at(i));

            findTraversals(curveXSize, curveYSize, x + 1, y + 1, false, traversals, path);
        }

    } else {
        findTraversals(curveXSize, curveYSize, x + 1, y + 1, false, traversals, inheritedPath);

        auto *path = new std::vector<Position>;

        for (int i = 0; i < inheritedPath->size(); i++)
            path->push_back(inheritedPath->at(i));

        findTraversals(curveXSize, curveYSize, x, y + 1, true, traversals, path);

    }

}