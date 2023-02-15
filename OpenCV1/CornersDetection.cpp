//
// Created by amelie on 14/02/23.
//

#include "CornersDetection.h"

vector<Point> cornerHarrisDetection(const Mat& inputImg) {
    //Parameters for the detector
    int thresh = 200;
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;
    Mat dst = Mat::zeros( inputImg.size(), CV_32FC1 );

    vector<Point> listCornersCoordinates;

    cornerHarris( inputImg, dst, blockSize, apertureSize, k);
    Mat dst_norm, dst_norm_scaled;
    normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
    convertScaleAbs( dst_norm, dst_norm_scaled );
    for( int i = 0; i < dst_norm.rows ; i++ )
    {
        for( int j = 0; j < dst_norm.cols; j++ )
        {
            int valueToTest = (int) dst_norm.at<float>(i,j);
            if( valueToTest > thresh )
            {
                circle( dst_norm_scaled, Point(j,i), 5,  Scalar(0), 2, 8, 0 );
                listCornersCoordinates.push_back(Point(j, i));
            }
        }
    }

    //namedWindow( "Corners detected" );
    //imshow( "Corners detected", dst_norm );

    // Uncomment following line to see the detected corners displayed on each picture
//    waitKey(0);

    return listCornersCoordinates;
}

vector<Point> goodFeaturesToTrackDetection(const Mat& inputImg) {
    vector<Point> corners;
    double qualityLevel = 0.01;
    double minDistance = 10;
    int blockSize = 3, gradientSize = 3;
    bool useHarrisDetector = false;
    double k = 0.04;

    Mat copy = inputImg.clone();
    goodFeaturesToTrack( inputImg,
                         corners,
                         maxCorners,
                         qualityLevel,
                         minDistance,
                         Mat(),
                         blockSize,
                         gradientSize,
                         useHarrisDetector,
                         k );
    int radius = 4;
    for(const auto & corner : corners) {
        circle( copy, corner, radius, Scalar(0, 255, 0), FILLED );
    }
    //namedWindow( "Corners detected" );
    //imshow( "Corners detected", copy );

    // Uncomment following line to see the detected corners displayed on each picture
//    waitKey(0);

    return corners;
}

bool cmpDistToUpperLeft(const Point& p1, const Point& p2){
    return ((pow(p1.y,2)+pow(p1.x,2)) < (pow(p2.y,2)+pow(p2.x,2)));
}