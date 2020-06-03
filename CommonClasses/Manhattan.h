#ifndef EMIRIS1_MANHATTAN_H
#define EMIRIS1_MANHATTAN_H

#include <vector>
#include <cmath>

struct {
    template<class T>
    double operator()(const std::vector<T> &v1, const std::vector<T> &v2) {
        double distance = 0, newComponent, temp;

        //Check dimensions
        if (v1.size() == 0 || v2.size() == 0 || v1.size() != v2.size()) {
            std::cout << "Invalid dimension" << std::endl;
            return -1;
        }

        for (int i = 0; i < v1.size(); i++) {
            newComponent = v1[i] - v2[i];

            temp = std::fabs(newComponent);

            distance += temp;
        }

        return distance;
    }
}ManhattanDistance;

#endif //EMIRIS1_MANHATTAN_H
