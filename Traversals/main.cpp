#include <iostream>
#include <vector>

#include "../CommonClasses/Curve/Curve.h"
#include "../CommonClasses/FileUtils/FileUtils.h"
#include "Position.h"
#include "Traversals.h"


int main() {

    std::string inputFile = "../../Dataset/Trajectory/traversal_dataset";
    std::vector<Curve> curveDataset;
    int maxCurvePoints = readCurveDataset(inputFile, curveDataset);

    std::vector<std::vector<Position>> traversals;
    std::vector<std::vector<Position>> *MxM[10][10];

    for (int i = 1; i <= 10; i++) {
        for (int j = 1; j <= 10; j++) {
            auto *inheritedPath = new std::vector<Position>;
            findTraversals(i, j, 1, 1, false, traversals, inheritedPath);
            MxM[i-1][j-1] = &traversals;
        }
    }

    return 0;
}