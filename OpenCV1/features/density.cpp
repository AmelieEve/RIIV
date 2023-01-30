#include "density.h"

vector<double> density::operator()(const Mat& inputImg)
{
	double meanGrayValue = mean(inputImg)[0];
	values.push_back(meanGrayValue);
	return values;
}
