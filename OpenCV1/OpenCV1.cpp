

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
#define size_list String("{small,medium,large}")

constexpr auto X_ZONING = 3;
constexpr auto Y_ZONING = 3;

using namespace cv;
using namespace std;
namespace fs = filesystem;

double mean(const list<pair<double, pair<double, double>>> dlist) {
    double acc = 0.0; 

    for (const pair<double, pair<double, double>> dou : dlist) {
        acc += dou.first;
    }

    return acc / dlist.size();
}

double standardDeviation(const list<pair<double, pair<double, double>>> dlist) {
    double localMean = mean(dlist);
    double acc = 0.0;
     
    for (const pair<double, pair<double, double>> dou : dlist) {
        acc += pow(dou.first - localMean, 2);
    }

    return acc / dlist.size();
}

pair<double, double> meanCenterOfMass(const list<pair<double, pair<double, double>>> dlist) {
    double accX = 0.0, accY = 0.0;
    for (const pair<double, pair<double, double>> dou : dlist) {
        accX += dou.second.first;
        accY += dou.second.second;
    }

    double meanX = accX / dlist.size(), meanY = accY / dlist.size();
    return {meanX, meanY};
}

pair<double, double> standardDeviationCenterOfMass(const list<pair<double, pair<double, double>>> dlist) {
    pair<double, double> localMean = meanCenterOfMass(dlist);
    double accX = 0.0, accY = 0.0;
    for (const pair<double, pair<double, double>> dou : dlist) {
        accX += pow(dou.second.first - localMean.first, 2);
        accY += pow(dou.second.second - localMean.second, 2);
    }

    double meanX = accX / dlist.size(), meanY = accY / dlist.size();
    return { meanX, meanY };
}

int main()
{
    Mat originalImage;
    Mat grayscaleImage;
    Mat binaryImage;
    Mat croppedImage;
    Mat zonedImage;

    String imgsPath = "../images/exemples";
    String txtsPath = "../images/exemples_txt/";
    ofstream arffFile("results.arff");

    list<pair<String, String>> attrList;
    attrList.push_back({ String("symbol"), symbol_list});
    attrList.push_back({ String("size"), size_list});
    attrList.push_back({ String("height"), num});
    attrList.push_back({ String("width"), num});
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

    map<String, list<pair<double,pair<double, double>>>> map;

    for (const auto& entry : fs::directory_iterator(imgsPath)) {
        String fileName = entry.path().filename().string();
        fileName = fileName.substr(0, fileName.find(".")) + ".txt";

        originalImage =  imread(entry.path().string());
        ifstream infile(txtsPath + fileName);

        list<String> attributes;

        String lineLabel, lineSize;
        getline(infile, lineLabel);
        getline(infile, lineLabel);
        getline(infile, lineSize);
        getline(infile, lineSize);
        getline(infile, lineSize);
        getline(infile, lineSize);
        getline(infile, lineSize);
        getline(infile, lineSize);

        istringstream iss1(lineLabel);
        String label, actualLabel;
        iss1 >> label >> actualLabel;
        attributes.push_back(actualLabel);

        istringstream iss2(lineSize);
        String size;
        iss2 >> label >> size;

        infile.close();

        if (size == "") {
            size = "medium";
        }

        attributes.push_back(size);

        cvtColor(originalImage, grayscaleImage, COLOR_BGR2GRAY);
        threshold(grayscaleImage, binaryImage, 200, 255, THRESH_BINARY);
        cropFromContour(grayscaleImage, croppedImage);

        if (croppedImage.rows == 0 || croppedImage.cols == 0) {
            continue;
        }

        attributes.push_back(to_string(croppedImage.rows));
        attributes.push_back(to_string(croppedImage.cols));

        Size newSize = Size(128, 128);
        resize(croppedImage, croppedImage, newSize);

        Scalar meanGrayValue = mean(croppedImage);
        attributes.push_back(to_string(meanGrayValue[0]));

        pair<double, double> centerOfMass = contourCenterOfMass(croppedImage);
        attributes.push_back(to_string(centerOfMass.first));
        attributes.push_back(to_string(centerOfMass.second));

        if (map.find(actualLabel + " " + size) == map.end()) {
            list<pair<double, pair<double, double>>> dlist;
            dlist.push_back({ meanGrayValue[0], centerOfMass });
            map.insert(make_pair(actualLabel + " " + size, dlist));
            cout << "new entry : " << actualLabel + " " + size << endl;
        }
        else {
            map.find(actualLabel + " " + size)->second.push_back({ meanGrayValue[0], centerOfMass });
        }

        for (int i = 0; i < X_ZONING; i++) {
            for (int j = 0; j < Y_ZONING; j++) {
                zonedImage = croppedImage(Range(36 * i, 36 * i + 56), Range(36 * j, 36 * j + 56));
                meanGrayValue = mean(zonedImage);
                attributes.push_back(to_string(meanGrayValue[0]));
                    
                centerOfMass = contourCenterOfMass(zonedImage);
                attributes.push_back(to_string(centerOfMass.first));
                attributes.push_back(to_string(centerOfMass.second));
            }
        }

        arffFile << generateARFFLine(attributes);
    }
    arffFile.close();
    for (const auto& element : map) {
        list<pair<double, pair<double, double>>> dlist = element.second;
        pair<pair<double, double>, pair<double, double>> centerOfMass = { meanCenterOfMass(dlist), standardDeviationCenterOfMass(dlist) };
        cout << element.first << " : " << mean(dlist) << " " << standardDeviation(dlist) << " (" << centerOfMass.first.first << ", " << centerOfMass.first.second << ") (" << centerOfMass.second.first << ", " << centerOfMass.second.second << ")" << endl;
    }

    std::exit(0);
}

