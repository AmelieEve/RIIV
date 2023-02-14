//
// Created by amelie on 14/02/23.
//

#ifndef RIIV_STATS_H
#define RIIV_STATS_H

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

double mean(const list<pair<double, pair<double, double>>>& dlist);

double standardDeviation(const list<pair<double, pair<double, double>>>& dlist);

pair<double, double> meanCenterOfMass(const list<pair<double, pair<double, double>>>& dlist);

pair<double, double> standardDeviationCenterOfMass(const list<pair<double, pair<double, double>>>& dlist);

#endif //RIIV_STATS_H