#ifndef CONTOURS_H
#define CONTOURS_H

#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

/*
 * Calcule le centre de gravit� d'une image pass�e en param�tre 
 */
pair<double, double> contoursCenterOfMass(const Mat& inputImg);
#endif // !CONTOURS_H
