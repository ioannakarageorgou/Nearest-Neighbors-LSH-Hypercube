#include <iostream>
#include "CurveToVector.h"


CurveToVector::CurveToVector(int L, int dim, std::string id) {

    this->L = L;
    this->id = id;

    this->G.reserve(L);         //array of L Grids
    for(int i=0; i<L;i++){
        this->G.emplace_back(0.00005f); //0.00005f
    }
}


CurveToVector::~CurveToVector() {
}


//curve ( (x1,y1), (x2, y2), (x3, y3),...)
//concatenated (x1, y1, x2, y2, x3, y4, ...)
std::vector<double> * CurveToVector::createGridCurveVector(std::vector<Point > v, int& index){

    std::vector<double> * gridCurve;

    gridCurve = this->G[index].createGridCurve(v);         //create grid curve
    for(double n: *gridCurve){
  //      std::cout<<n<<" ";
    }
 //   std::cout<<std::endl<<"----------------------------------------------------------------------------------------------------------\n";
    return gridCurve;
}


//Insert a L grid curves in L LSH HashTables
void CurveToVector::LshInsert(Curve &curve) {

    for(int i=0; i<this->L; i++){
        std::vector<double>* gridCurve = createGridCurveVector(curve.getComponents(),i);

        DataVector d(id,*gridCurve);
        ////TODO
        curve.addGridVector(new DataVector(id, *gridCurve));
    }
}