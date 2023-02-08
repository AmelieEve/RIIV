#pragma once
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Feature
{
protected :
    /**
     * Container for the values computed by calling operator()
     */
	vector<double> values;
    /**
     * Name of the feature to be used in generation of arff file portions
     **/
	String name;

public : 
	Feature(String n);
    vector<double> getValues() { return values; }
	virtual vector<double> operator() (const Mat& inputImg) = 0;
	vector<pair<String, String>> generateArffHeaderPortion();
	vector<String> generateArffLinePortion();
};

