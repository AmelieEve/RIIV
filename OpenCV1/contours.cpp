#include "contours.h"

Point center;

struct contour_greater
{
    bool operator() (const Point& a, const Point& b) {
        double angle1 = atan2(a.y - center.y, a.x - center.x) * 180.0 / CV_PI;
        double angle2 = atan2(b.y - center.y, b.x - center.x) * 180.0 / CV_PI;
        return (angle1 < angle2);
    }
};

template<typename T>
vector<T> flatten(const vector<vector<T>>& vec) {
    vector<T> result;
    for (auto const& v : vec) {
        result.insert(result.end(), v.begin(), v.end());
    }
    return result;
}

void cropFromContour(const Mat& inputGrayImg, Mat& outputImg)
{
    Mat workingImage = inputGrayImg.clone();
    vector<vector<Point>> contours;

    Canny(workingImage, workingImage, 0, 230);
    findContours(workingImage, contours, RETR_LIST, CHAIN_APPROX_SIMPLE, Point(0, 0));

    vector<Point> allcontours = flatten(contours);
    sort(allcontours.begin(), allcontours.end(), contour_greater());

    Rect rectangle = boundingRect(allcontours);

    outputImg = inputGrayImg(rectangle);
}

pair<double, double> contourAreaAndPerimeter(const Mat& inputGrayImg)
{
    Mat workingImage = inputGrayImg.clone();
    vector<vector<Point>> contours;

    Canny(workingImage, workingImage, 0, 230);
    findContours(workingImage, contours, RETR_LIST, CHAIN_APPROX_SIMPLE, Point(0, 0));

    vector<Point> allcontours = flatten(contours);
    sort(allcontours.begin(), allcontours.end(), contour_greater());

    return { contourArea(allcontours), arcLength(allcontours, true) };
}

double equivalentRadius(const Mat& inputGrayImg)
{
    Mat workingImage = inputGrayImg.clone();
    vector<vector<Point>> contours;

    Canny(workingImage, workingImage, 0, 230);
    findContours(workingImage, contours, RETR_LIST, CHAIN_APPROX_SIMPLE, Point(0, 0));

    vector<Point> allcontours = flatten(contours);
    sort(allcontours.begin(), allcontours.end(), contour_greater());


    if (allcontours.size() < 1) return 0.0;
    return sqrt(contourArea(allcontours) / CV_PI);
}
