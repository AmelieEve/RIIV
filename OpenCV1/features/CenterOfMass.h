//
// Created by amelie on 30/01/23.
//

#ifndef RIIV_CENTEROFMASS_H
#define RIIV_CENTEROFMASS_H

#include "Feature.h"

class CenterOfMass : public Feature {
public:
    CenterOfMass(String n);
    vector<double> operator() (const Mat& inputImg) override;
};


#endif //RIIV_CENTEROFMASS_H
