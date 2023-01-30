//
// Created by amelie on 30/01/23.
//

#ifndef RIIV_CENTEROFMASS_H
#define RIIV_CENTEROFMASS_H

#include "Feature.h"

/*
 * Calcule le centre de gravite d'une image passee en parametre
 */
class CenterOfMass : public Feature {
public:
    CenterOfMass(String n);
    vector<double> operator() (const Mat& inputImg) override;
};


#endif //RIIV_CENTEROFMASS_H
