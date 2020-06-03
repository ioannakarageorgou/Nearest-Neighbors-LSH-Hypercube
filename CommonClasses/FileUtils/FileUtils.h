#ifndef PROJECT_EMIRIS_FILEUTILS_H
#define PROJECT_EMIRIS_FILEUTILS_H

#include <string>
#include <vector>
#include "../../CommonClasses/DataVector/DataVector.h"
#include "../Curve/Curve.h"
#include "../CurveEntry/CurveEntry.h"

class NNResult;

void readVectorDataset(const std::string &fileName, std::vector<DataVector> &vectorDataset);

bool fileExist(const std::string &fileName);

void exportDatavectorResults(const std::string &fileName, const std::vector<NNResult *> &exactNNResults,
                   const std::vector<NNResult *> &approximateNNResults, int radius);

void exportCurveResults(const std::string &fileName, const std::vector<NNResult *> &exactNNResults,
                        const std::vector<NNResult *> &approximateNNResults, int L);

int readCurveDataset(const std::string &fileName,std::vector<Curve>& curveDataset );

int calculateW(const std::vector<NNResult *> &exactNNResults);

double calculateD(std::vector<CurveEntry> &vectorDataset);

//readCurveDataset();

#endif
