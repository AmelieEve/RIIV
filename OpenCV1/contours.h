#ifndef CONTOURS_H
#define CONTOURS_H

#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

/*
 * Calcule le centre de gravité d'une image passée en paramètre 
 */
pair<double, double> contourCenterOfMass(const Mat& inputImg);

/* 
 * Retourne une image croppée par rapport à ses contours
 */
void cropFromContour(const Mat& inputGrayImg, Mat& outputImg);
#endif // !CONTOURS_H
