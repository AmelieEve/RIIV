#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <filesystem>
#include "contours.h"
#include "creationARFF.h"

#define num String("NUMERIC")
#define symbol_list String("{accident,bomb,car,casualty,electricity,fire,firebrigade,flood,gas,injury,paramedics,person,police,roadblock}")

constexpr auto X_ZONING = 3;
constexpr auto Y_ZONING = 3;
int maxCorners = 10;

using namespace cv;
using namespace std;
namespace fs = filesystem;

double mean(const list<pair<double, pair<double, double>>> dlist) {
    double acc = 0.0; 

    auto d_front = dlist.begin();
    for (int i = 0; i < dlist.size(); i++) {
        acc += (*d_front).first;
        advance(d_front, 1);
    }

    return acc / dlist.size();
}

double standardDeviation(const list<pair<double, pair<double, double>>> dlist) {
    double localMean = mean(dlist);
    double acc = 0.0;
     
    auto d_front = dlist.begin();
    for (int i = 0; i < dlist.size(); i++) {
        acc += pow((*d_front).first - localMean, 2);
        advance(d_front, 1);
    }

    return acc / dlist.size();
}

pair<double, double> meanCenterOfMass(const list<pair<double, pair<double, double>>> dlist) {
    double accX = 0.0, accY = 0.0;

    auto d_front = dlist.begin();
    for (int i = 0; i < dlist.size(); i++) {
        accX += (*d_front).second.first;
        accY += (*d_front).second.second;
        advance(d_front, 1);
    }

    double meanX = accX / dlist.size(), meanY = accY / dlist.size();
    return {meanX, meanY};
}

pair<double, double> standardDeviationCenterOfMass(const list<pair<double, pair<double, double>>> dlist) {
    pair<double, double> localMean = meanCenterOfMass(dlist);
    double accX = 0.0, accY = 0.0;

    auto d_front = dlist.begin();
    for (int i = 0; i < dlist.size(); i++) {
        accX += pow((*d_front).second.first - localMean.first, 2);
        accY += pow((*d_front).second.second - localMean.second, 2);
        advance(d_front, 1);
    }

    double meanX = accX / dlist.size(), meanY = accY / dlist.size();
    return { meanX, meanY };
}

vector<double> centerOfMassComputing(const Mat& inputImg) {
    Moments m = moments(inputImg);
    return {m.m10 / m.m00, m.m01 / m.m00};
}

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

    namedWindow( "Corners detected" );
    imshow( "Corners detected", dst_norm );
//    waitKey(0);
    return listCornersCoordinates;
}

vector<Point> goodFeaturesToTrack(const Mat& inputImg)
{
    vector<Point> corners;
    double qualityLevel = 0.01;
    double minDistance = 10;
    int blockSize = 3, gradientSize = 3;
    bool useHarrisDetector = false;
    double k = 0.04;
    RNG rng(12345);

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
    cout << "** Number of corners detected: " << corners.size() << endl;
    int radius = 4;
    for( size_t i = 0; i < corners.size(); i++ )
    {
        circle( copy, corners[i], radius, Scalar(rng.uniform(0,255), rng.uniform(0, 256), rng.uniform(0, 256)), FILLED );
    }
    namedWindow( "Corners detected" );
    imshow( "Corners detected", copy );
//    waitKey(0);
    return corners;
}

bool cmpDistToUpperLeft(const Point& p1, const Point& p2){
    return ((pow(p1.y,2)+pow(p1.x,2)) < (pow(p2.y,2)+pow(p2.x,2)));
}

int main()
{
    /**
     * INITIALIZATION
     **/

    //Mat containers
    Mat originalImage;
    Mat grayscaleImage;
    Mat binaryImage;
    Mat croppedImage;
    Mat zonedImage;

    //Files and paths
    String imgsPath = "../images/exemples";
    ofstream arffFile("results.arff");

    //ARFF file header
    list<pair<String, String>> attrList;
    attrList.push_back({ String("symbol"), symbol_list});
    attrList.push_back({ String("height"), num});
    attrList.push_back({ String("width"), num});
    attrList.push_back({ String("diagonalLength"), num });
    attrList.push_back({ String("contourArea"), num });
    attrList.push_back({ String("contourPerimeter"), num });
    attrList.push_back({ String("equivalent_radius"), num });
    attrList.push_back({ String("mean_gray"), num});
    attrList.push_back({ String("center_of_mass_X"), num});
    attrList.push_back({ String("center_of_mass_Y"), num});

    for (int i = 0; i < X_ZONING; i++) {
        for (int j = 0; j < Y_ZONING; j++) {
            attrList.push_back({ String("mean_gray_zone_" + to_string(i) + "_" + to_string(j) ), num});
            attrList.push_back({ String("center_of_mass_X_zone_" + to_string(i) + "_" + to_string(j) ), num});
            attrList.push_back({ String("center_of_mass_Y_zone_" + to_string(i) + "_" + to_string(j) ), num});
        }
    }

    /*
    for(int i = 0; i < maxCorners; i++) {
        attrList.push_back({String("corner_x_"+ to_string(i)), num});
        attrList.push_back({String("corner_y_"+ to_string(i)), num});
    }
    */

    arffFile << generateARFFHeader("results", attrList);

    int arffGenerationStep = 0;

    /**
     * FEATURES EXTRACTION
     **/
    for (const auto& entry : fs::directory_iterator(imgsPath)) {
        // File reading
        String fileName = entry.path().filename().string();
        fileName = fileName.substr(0, fileName.find("_"));

        originalImage =  imread(entry.path().string());

        list<String> attributes;

        // Get label
        attributes.push_back(fileName);


        // Binarization and cropping from contour
        cvtColor(originalImage, grayscaleImage, COLOR_BGR2GRAY);
        threshold(grayscaleImage, binaryImage, 200, 255, THRESH_BINARY);
        cropFromContour(grayscaleImage, croppedImage);

        // Test to avoid processing white images
        if (croppedImage.rows == 0 || croppedImage.cols == 0) {
            continue;
        }

        // Contour's height
        attributes.push_back(to_string(croppedImage.rows));
        // Contour's width
        attributes.push_back(to_string(croppedImage.cols));

        // Contour diagonal length
        attributes.push_back(to_string(sqrt(pow(croppedImage.cols, 2) + pow(croppedImage.rows, 2))));

        // Area and perimeter
        pair<double, double> surfaces = contourAreaAndPerimeter(grayscaleImage);
        attributes.push_back(to_string(surfaces.first));
        attributes.push_back(to_string(surfaces.second));

        // Equivalent circle radius
        double rad = equivalentRadius(croppedImage);
        attributes.push_back(to_string(rad));

        if (rad == 0.0) continue;

        // Size normalization
        Size newSize = Size(128, 128);
        resize(croppedImage, croppedImage, newSize);

        /*
         * Features
         */
        // Density
        Scalar meanGrayValue = mean(croppedImage);
        attributes.push_back(to_string(meanGrayValue[0]));

        // Center of mass
        vector<double> centerOfMass = centerOfMassComputing(croppedImage);
        attributes.push_back(to_string(centerOfMass[0]));
        attributes.push_back(to_string(centerOfMass[1]));

        // Fuzzy zoning
        for (int i = 0; i < X_ZONING; i++) {
            for (int j = 0; j < Y_ZONING; j++) {
                zonedImage = croppedImage(Range(36 * i, 36 * i + 56), Range(36 * j, 36 * j + 56));

                // Density
                meanGrayValue = mean(zonedImage);
                attributes.push_back(to_string(meanGrayValue[0]));

                // Center of mass
                centerOfMass = centerOfMassComputing(zonedImage);
                attributes.push_back(to_string(centerOfMass[0]));
                attributes.push_back(to_string(centerOfMass[1]));
            }
        }

        // Corners
        /*
        //vector<Point> listCornersCoordinates = cornerHarrisDetection(croppedImage);

        vector<Point> listCornersCoordinates = goodFeaturesToTrack(croppedImage);
        std::sort(listCornersCoordinates.begin(), listCornersCoordinates.end(), cmpDistToUpperLeft);
        for(const Point& p : listCornersCoordinates){
            attributes.push_back(to_string(p.x));
            attributes.push_back(to_string(p.y));
        }
        if(listCornersCoordinates.size() < maxCorners){
            int missingCorners = maxCorners - listCornersCoordinates.size();
            for(int i = 0; i < missingCorners; i++){
                attributes.push_back("?");
                attributes.push_back("?");
            }
        }
        */

        arffFile << generateARFFLine(attributes);

        arffGenerationStep++;
        cout << to_string(arffGenerationStep) << " " << fileName << endl;
    }
    arffFile.close();

    std::exit(0);
}

