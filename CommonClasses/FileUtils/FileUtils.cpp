#include <sstream>
#include <unistd.h>
#include <iostream>
#include <cassert>
#include <fstream>
#include <cfloat>
#include <cmath>


#include "FileUtils.h"
#include "../Utils.h"
#include "../DataVector/DataVector.h"
#include "../NNResult/NNResult.h"
#include "../Curve/Curve.h"
#include "../CurveEntry/CurveEntry.h"

void readVectorDataset(const std::string &fileName, std::vector<DataVector> &vectorDataset) {
    ASSERT(fileExist(fileName) && "File doesn't exist");
    ASSERT(vectorDataset.empty() && "vectorDataset is not empty");
    std::ifstream file;
    std::string line;
    std::string id;

    file.open(fileName);

    while (getline(file, line)) {
        ////TODO
        auto *components = new std::vector<double>();
        std::istringstream buffer(line);

        buffer >> id;
        for (std::string word; buffer >> word;) {
            components->push_back(stoi(word));
        }

        vectorDataset.emplace_back(id, *components);
    }
}

bool fileExist(const std::string &fileName) {
    return (access(fileName.c_str(), F_OK) != -1);
}

int readCurveDataset(const std::string &fileName, std::vector<Curve> &curveDataset) {
    ASSERT(fileExist(fileName) && "File doesn't exist");
    ASSERT(curveDataset.empty() && "vectorDataset is not empty");
    std::ifstream file;
    std::string line;
    std::string id;
    std::string pointQuantity;

    int maxPoints = 0;
    file.open(fileName);

    while (getline(file, line)) {
        ///TODO
        auto *components = new std::vector<Point>();
        std::istringstream buffer(line);

        buffer >> id;
        buffer >> pointQuantity;


        if (maxPoints < stoi(pointQuantity)) {
            maxPoints = stoi(pointQuantity);
        }

        for (std::string word; buffer >> word;) {

            std::string x, y;

            x = word.substr(1, word.size() - 2);

            buffer >> word;

            y = word.substr(0, word.size() - 2);

            components->emplace_back(atof(x.c_str()), atof(y.c_str()));
        }

        curveDataset.emplace_back(*components, id);

    }
    return maxPoints;
}


void exportDatavectorResults(const std::string &fileName, const std::vector<NNResult *> &exactNNResults,
                             const std::vector<NNResult *> &approximateNNResults, int radius) {
    ASSERT(!fileExist(fileName) && "Output File Already Exist");
    ASSERT(!exactNNResults.empty() && "There is no result to export");
    ASSERT(!approximateNNResults.empty() && "There is no result to export");

    std::ofstream output(fileName);

    int counter = 0;

    for (int i = 0; i < exactNNResults.size() && i < approximateNNResults.size(); i++) {
        if (exactNNResults.at(i)->getClosestNeighbour().begin()->second ==
            approximateNNResults.at(i)->getClosestNeighbour().begin()->second)
            counter++;
    }

    output << "------------------>" << counter << " CORRECT OF " << approximateNNResults.size()
           << " <------------------" << "\n\n";

    float stat = 0;

    for (int i = 0; i < exactNNResults.size() && i < approximateNNResults.size(); i++) {
        output << "==============================================================================\n";
        output << "Query: " << approximateNNResults.at(i)->getQueryId() << std::endl;
        output << "Nearest neighbor: " << approximateNNResults.at(i)->getClosestNeighbour().begin()->first
               << std::endl;
        output << "distanceLSH: " << approximateNNResults.at(i)->getClosestNeighbour().begin()->second << std::endl;
        output << "distanceTrue: " << exactNNResults.at(i)->getClosestNeighbour().begin()->second << std::endl;
        output << "tLSH: " << approximateNNResults.at(i)->getExecutionTime().count() << std::endl;
        output << "tTrue: " << exactNNResults.at(i)->getExecutionTime().count() << std::endl;

        float temp = (approximateNNResults.at(i)->getClosestNeighbour().begin()->second)/(exactNNResults.at(i)->getClosestNeighbour().begin()->second);
        if(temp > stat){
            stat = temp;
        }

        if (radius != 0) {
            std::map<std::string, double>::iterator it;
            output << "R-near neighbors: \n";
            for (it = approximateNNResults.at(i)->getClosestNeighbour().begin();
                 it != approximateNNResults.at(i)->getClosestNeighbour().end(); it++) {
                output << it->first << std::endl;
            }
        }
    }
    output.close();
    std::cout<<"Appr Dist / Real Dist = "<< stat << std::endl;
}

void exportCurveResults(const std::string &fileName, const std::vector<NNResult *> &exactNNResults,
                        const std::vector<NNResult *> &approximateNNResults, int L ) {
    ASSERT(!fileExist(fileName) && "Output File Already Exist");
    ASSERT(!exactNNResults.empty() && "There is no result to export");
    ASSERT(!approximateNNResults.empty() && "There is no result to export");

    std::ofstream output(fileName);

    int counter = 0;

    std::vector<NNResult> bestApproximateResults;
    NNResult dummyResult;
    dummyResult.addClosestNeighbour("Dummy",DBL_MAX);
    NNResult* bestResult = &dummyResult;
    int nResult= 1;
    for(int i = 0;i < approximateNNResults.size() ; i++){

        if(bestResult->getClosestNeighbour().begin()->second > approximateNNResults[i]->getClosestNeighbour().begin()->second){
            bestResult = approximateNNResults[i];
        }

        if(nResult == L ){
            bestApproximateResults.emplace_back(*bestResult);
            nResult = 1;
            bestResult=&dummyResult;
            continue;
        }
        nResult++;
    }

    for (int i = 0; i < exactNNResults.size() && i < bestApproximateResults.size(); i++) {
        if (exactNNResults.at(i)->getClosestNeighbour().begin()->second ==
                bestApproximateResults.at(i).getClosestNeighbour().begin()->second)
            counter++;
    }

    output << "------------------>" << counter << " CORRECT OF " << bestApproximateResults.size()
           << " <------------------" << "\n\n";

    float stat = 100000;

    for (int i = 0; i < exactNNResults.size() && i < bestApproximateResults.size(); i++) {
        output << "==============================================================================\n";
        output << "Query: " << bestApproximateResults.at(i).getQueryId() << std::endl;
        output << "Nearest neighbor: " << bestApproximateResults.at(i).getClosestNeighbour().begin()->first
               << std::endl;
        output << "distanceLSH: " << bestApproximateResults.at(i).getClosestNeighbour().begin()->second << std::endl;
        output << "distanceTrue: " << exactNNResults.at(i)->getClosestNeighbour().begin()->second << std::endl;
        output << "tLSH: " << bestApproximateResults.at(i).getExecutionTime().count() << std::endl;
        output << "tTrue: " << exactNNResults.at(i)->getExecutionTime().count() << std::endl;

        if(bestApproximateResults.at(i).getClosestNeighbour().begin()->second != 0 && exactNNResults.at(i)->getClosestNeighbour().begin()->second != 0 && std::isfinite(exactNNResults.at(i)->getClosestNeighbour().begin()->second) && std::isfinite(bestApproximateResults.at(i).getClosestNeighbour().begin()->second)){
            float temp = (bestApproximateResults.at(i).getClosestNeighbour().begin()->second)/(exactNNResults.at(i)->getClosestNeighbour().begin()->second);
            if(temp < stat && temp != 1){
                stat = temp;
            }
        }

    }
    output.close();
    std::cout<<"Appr Dist / Real Dist = "<< stat << std::endl;
}

int calculateW(const std::vector<NNResult *> &exactNNResults){
    // 4 times the average distance between the real closest neighbors
    float sumD=0;
    for(int i =0; i< exactNNResults.size(); i++){
        sumD += exactNNResults.at(i)->getClosestNeighbour().begin()->second;
    }
    int w = 4 * (sumD/exactNNResults.size());
    return w;
}

//
//double calculateD(std::vector<CurveEntry> &vectorDataset){
//
//    double avgD = 0;
//    std::vector<CurveEntry>::iterator iterCurves;
//    //for each curve
//    for(iterCurves = vectorDataset.begin(); iterCurves< vectorDataset.end(); iterCurves++){
//        std::vector<Point> curvePoints = iterCurves->getComponents();
//
//        std::vector<Point>::iterator iterPoints;
//        double tempD = 0;
//        for(iterPoints = curvePoints.begin(); iterPoints <curvePoints.end(); iterPoints++){
//            Point tp = *iterPoints;
//            auto nx = std::next(iterPoints, 1);
//
//            tempD += sqrt(pow(tp.getx() - nx->getx(), 2) + pow(tp.gety() - nx->gety(), 2));
//        }
//        //std::cout<<"avgD = "<< avgD << std::endl;
//        avgD += tempD/curvePoints.size();
//    }
//
////    return avgD/vectorDataset.size();
//        return avgD;
//}

//double calculateD(std::vector<CurveEntry> &vectorDataset){
//
//    double avgD = 0;
//    std::vector<CurveEntry>::iterator iterCurves;
//    //for each curve
//    for(iterCurves = vectorDataset.begin(); iterCurves< vectorDataset.end(); iterCurves++){
//        std::vector<Point> curvePoints = iterCurves->getComponents();
//
//        std::vector<Point>::iterator iterPoints;
//        double tempD = 0;
//        for(iterPoints = curvePoints.begin(); iterPoints <curvePoints.end(); iterPoints++){
//            Point tp = *iterPoints;
//            auto nx = std::next(iterPoints, 1);
//
//            tempD += sqrt(pow(tp.getx() - nx->getx(), 2) + pow(tp.gety() - nx->gety(), 2));
//        }
//        //std::cout<<"avgD = "<< avgD << std::endl;
//        avgD += tempD/curvePoints.size();
//    }
//
////    return avgD/vectorDataset.size();
//        return avgD;
//}
