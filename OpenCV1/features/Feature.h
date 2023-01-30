#pragma once
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Feature
{
private :
	vector<double> values;
	String name;

public : 
	Feature(String n);
	virtual vector<double> operator() (const Mat& inputImg) = 0;
	vector<pair<String, String>> generateArffHeaderPortion();
	vector<String> generateArffLinePortion();
};

