#pragma once

#include "Feature.h"
#include <vector>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

class Density : public Feature {
public:
    Density(String n);
    vector<double> operator() (const Mat& inputImg) override;
};

