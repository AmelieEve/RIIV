#include "contours.h"

pair<double, double> contourCenterOfMass(const Mat& inputImg)
{
    Moments m = moments(inputImg);
    return { m.m10 / m.m00, m.m01 / m.m00 };
}

void cropFromContour(const Mat& inputGrayImg, Mat& outputImg)
{
    Mat workingImage = inputGrayImg.clone();
    vector<vector<Point>> contours;

    Canny(workingImage, workingImage, 0, 230);
    findContours(workingImage, contours, RETR_LIST, CHAIN_APPROX_SIMPLE, Point(0, 0));

    vector<Point> outerContour;
    double maxContourArea = 0.0;
    for (vector<Point> cont : contours) {
        if (maxContourArea < contourArea(cont)) {
            outerContour = cont;
            maxContourArea = contourArea(cont);
        }
    }
    Rect rectangle = boundingRect(outerContour);

    outputImg = inputGrayImg(rectangle);
}
