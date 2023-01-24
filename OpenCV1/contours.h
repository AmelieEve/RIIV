#ifndef CONTOURS_H
#define CONTOURS_H

#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

/*
 * Calcule le centre de gravité d'une image passée en paramètre 
 */
pair<double, double> contoursCenterOfMass(const Mat& inputImg);
#endif // !CONTOURS_H
