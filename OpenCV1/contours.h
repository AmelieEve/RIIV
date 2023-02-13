#ifndef CONTOURS_H
#define CONTOURS_H

#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

/* 
 Retourne une image cropp�e par rapport � ses contours
 */
void cropFromContour(const Mat& inputGrayImg, Mat& outputImg);

/*
 Retourne la surface de l'enveloppe convexe de l'imagette
 */
pair<double, double> contourAreaAndPerimeter(const Mat& inputGrayImg);

/*
 Retourne le diamètre du cercle équivalent 
 */
double equivalentRadius(const Mat& inputGrayImg);

#endif // !CONTOURS_H
