#include "contours.h"

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

pair<double, double> contourAreaAndPerimeter(const Mat& inputGrayImg)
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

    return { contourArea(outerContour), arcLength(outerContour, true) };
}

double equivalentRadius(const Mat& inputGrayImg)
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


    if (outerContour.size() < 1) return 0.0;
    return sqrt(contourArea(outerContour) / acos(-1));
}
