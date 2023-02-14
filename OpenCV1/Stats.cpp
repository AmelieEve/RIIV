//
// Created by amelie on 14/02/23.
//

#include "Stats.h"

double mean(const list<pair<double, pair<double, double>>>& dlist) {
    double acc = 0.0;

    auto d_front = dlist.begin();
    for (int i = 0; i < dlist.size(); i++) {
        acc += (*d_front).first;
        advance(d_front, 1);
    }

    return acc / dlist.size();
}

double standardDeviation(const list<pair<double, pair<double, double>>>& dlist) {
    double localMean = mean(dlist);
    double acc = 0.0;

    auto d_front = dlist.begin();
    for (int i = 0; i < dlist.size(); i++) {
        acc += pow((*d_front).first - localMean, 2);
        advance(d_front, 1);
    }

    return acc / dlist.size();
}

pair<double, double> meanCenterOfMass(const list<pair<double, pair<double, double>>>& dlist) {
    double accX = 0.0, accY = 0.0;

    auto d_front = dlist.begin();
    for (int i = 0; i < dlist.size(); i++) {
        accX += (*d_front).second.first;
        accY += (*d_front).second.second;
        advance(d_front, 1);
    }

    double meanX = accX / dlist.size(), meanY = accY / dlist.size();
    return {meanX, meanY};
}

pair<double, double> standardDeviationCenterOfMass(const list<pair<double, pair<double, double>>>& dlist) {
    pair<double, double> localMean = meanCenterOfMass(dlist);
    double accX = 0.0, accY = 0.0;

    auto d_front = dlist.begin();
    for (int i = 0; i < dlist.size(); i++) {
        accX += pow((*d_front).second.first - localMean.first, 2);
        accY += pow((*d_front).second.second - localMean.second, 2);
        advance(d_front, 1);
    }

    double meanX = accX / dlist.size(), meanY = accY / dlist.size();
    return { meanX, meanY };
}