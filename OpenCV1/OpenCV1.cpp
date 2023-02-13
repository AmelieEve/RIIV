#include <iostream>
#include <fstream>
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

    // ARFF file header
    //TODO: generate header portions from old_features_modules classes' function
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
        //TODO: size normalization without deformation
        //TODO: define size (not hard-coded)
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

        arffFile << generateARFFLine(attributes);

        arffGenerationStep++;
        cout << to_string(arffGenerationStep) << " : " << fileName << endl;
    }
    arffFile.close();

    std::exit(0);
}

