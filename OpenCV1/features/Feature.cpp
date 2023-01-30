#include "Feature.h"

#define num String("NUMERIC")

Feature::Feature(String n)
{
	values = vector<double>();
	name = n;
}

vector<pair<String, String>> Feature::generateArffHeaderPortion()
{
	vector<pair<String, String>> result;
	int i = 0;

	for (double value : values)
	{
		result.push_back({ String(name + "_" + to_string(i)), num});
		i++;
	}

	return result;
}

vector<String> Feature::generateArffLinePortion()
{
	vector<String> result;
	int i = 0;

	for (double value : values)
	{
		result.push_back(String(to_string(value)));
		i++;
	}

	return result;
}
