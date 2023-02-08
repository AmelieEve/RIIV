#include "Density.h"

Density::Density(String n): Feature(n){}

vector<double> Density::operator()(const Mat& inputImg) {
	double meanGrayValue = mean(inputImg)[0];
	values.push_back(meanGrayValue);
	return values;
}
