#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <filesystem>
#include "contours.h"
#include "creationARFF.h"
#include "CornersDetection.h"

#define num String("NUMERIC")
#define symbol_list String("{accident,bomb,car,casualty,electricity,fire,firebrigade,flood,gas,injury,paramedics,person,police,roadblock}")

constexpr auto X_ZONING = 3;
constexpr auto Y_ZONING = 3;

using namespace cv;
using namespace std;
namespace fs = filesystem;

int main()
{
    /**
     * INITIALIZATION
     **/

    // Mat containers
    Mat originalImage;
    Mat grayscaleImage;
    Mat binaryImage;
    Mat croppedImage;
    Mat zonedImage;

    // Files and paths
    String imgsPath = "../images/exemples";
    ofstream arffFile("results.arff");

    // ARFF file header
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

    // Uncomment following loop to take corners into account
    // Don't forget to uncomment matching section of code in the "FEATURES EXTRACTION" part
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

        // Contour's diagonal length
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

        // Density
        Scalar meanGrayValue = mean(croppedImage);
        attributes.push_back(to_string(meanGrayValue[0]));

        // Center of mass
        Moments m = moments(croppedImage);
        vector<double> centerOfMass = {m.m10 / m.m00, m.m01 / m.m00};
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
                Moments mZone = moments(zonedImage);
                centerOfMass = { mZone.m10 / mZone.m00, mZone.m01 / mZone.m00 };
                attributes.push_back(to_string(centerOfMass[0]));
                attributes.push_back(to_string(centerOfMass[1]));
            }
        }

        // Corners
        // Uncomment following section to take corners into account

        /*
        vector<Point> listCornersCoordinates = goodFeaturesToTrackDetection(croppedImage);
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

