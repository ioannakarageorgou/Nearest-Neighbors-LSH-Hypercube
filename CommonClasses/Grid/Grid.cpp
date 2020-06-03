#include <chrono>
#include <random>
#include <iostream>
#include "Grid.h"

float getUniform(double limit) {

    static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::default_random_engine generator(seed);

    static std::uniform_real_distribution<float> uniformDist(0, limit);

    return uniformDist(generator);
}


Grid::Grid(double d) {         //Create a t~[0,d)^d
    int i;
    double x = 0;

    this->d = d;

    for (i = 0; i < 2; i++) {
        x = getUniform(0.00005f);
        this->t.push_back(x);
//        std::cout<<x<<std::endl;
    }
//    std::cout<<"----------------------------------"<<std::endl;
}


Grid::~Grid() {
    //?
}


std::vector<double> *Grid::createGridCurve(std::vector<Point> &v) {
    double prev;

    std::vector<double> *vec = new std::vector<double>();
    //std::cout<<"t[0] "<<t[0]<< " t[1] "<< t[1] << std::endl;
    for (int i = 0; i < v.size(); i++) {

//        Point p = v[i];
//        double temp1 = ((p.getx() + 0.5) / this->d) + this->t[0];
//        double temp2 = ((p.gety() + 0.5) / this->d) + this->t[1];
        double a1 = round((v[i].getx()/d)+t[0]);
        double a2 = round((v[i].gety()/d) + t[1]);
        if( i == 0){
           // std::cout<<std::endl<<"A1 "<<a1<<" A2 "<< a2 << std::endl;
        }
      //  std::cout << a1* d  << "  " << a2* d  << " ";
        if (prev != a1) {
            vec->push_back(a1);
            prev = a1 ;
        }
        if (prev != a2 ) {
            vec->push_back(a2);
            prev = a2;
        }
    }
    std::cout << std::endl;
    return vec;
}

const std::vector<double> &Grid::getT() const {
    return t;
}
