#ifndef CONTOURS_H
#define CONTOURS_H

#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

/* 
 * Retourne une image cropp�e par rapport � ses contours
 */
void cropFromContour(const Mat& inputGrayImg, Mat& outputImg);
#endif // !CONTOURS_H
