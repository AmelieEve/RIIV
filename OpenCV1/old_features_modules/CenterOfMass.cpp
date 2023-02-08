//
// Created by amelie on 30/01/23.
//

#include "CenterOfMass.h"
#include "../contours.h"

CenterOfMass::CenterOfMass(String n) : Feature(n) { }

vector<double> CenterOfMass::operator()(const Mat &inputImg) {
    Moments m = moments(inputImg);
    values.push_back(m.m10 / m.m00);
    values.push_back(m.m01 / m.m00);
    return values;
}