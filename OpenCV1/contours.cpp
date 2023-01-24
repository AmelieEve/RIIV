#include "contours.h"

pair<double, double> contoursCenterOfMass(const Mat& inputGrayImg)
{
    Mat outputContours = inputGrayImg.clone();
    vector<vector<Point>> contours;

    Canny(outputContours, outputContours, 0, 230);
    findContours(outputContours, contours, RETR_LIST, CHAIN_APPROX_SIMPLE, Point(0, 0));

    vector<Point> outerContour;
    double maxContourArea = 0.0;
    for (vector<Point> cont : contours) {
        if (maxContourArea < contourArea(cont)) {
            outerContour = cont;
            maxContourArea = contourArea(cont);
        }
    }
    Moments m = moments(outerContour);

    return { m.m10 / m.m00, m.m01 / m.m00 };
}
