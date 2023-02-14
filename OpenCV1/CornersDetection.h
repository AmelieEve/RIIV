//
// Created by amelie on 14/02/23.
//

#ifndef RIIV_CORNERSDETECTION_H
#define RIIV_CORNERSDETECTION_H

#include <opencv2/opencv.hpp>

#define maxCorners 10

using namespace cv;
using namespace std;

/*
 * First method to detect corners and get their coordinates (Harris) : simple call to function cv::cornerHarris()
 * Uncomment line "waitKey(0);" in implementation to see the detected corners displayed on each picture
 */
vector<Point> cornerHarrisDetection(const Mat& inputImg);

/*
 * Second method to detect corners and get their coordinates (Shi-Tomasi) : call to function cv::goodFeaturesToTrack()
 * Uncomment line "waitKey(0);" in implementation to see the detected corners displayed on each picture
 */
vector<Point> goodFeaturesToTrackDetection(const Mat& inputImg);

/*
 * Compare function called in main program to sort the output of corners detection
 * Sorts the points by their distance to the upper left corner of the image
 */
bool cmpDistToUpperLeft(const Point& p1, const Point& p2);

#endif //RIIV_CORNERSDETECTION_H
