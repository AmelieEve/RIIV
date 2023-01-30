#pragma once

#include "Feature.h"
#include <vector>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

class density :
    public Feature
{
    density() :Feature(String("name")) {}
    virtual vector<double> operator() (const Mat& inputImg);
};

