#include "Feature.h"

#define num String("NUMERIC")

Feature::Feature(String n) : name(n) {
	values = vector<double>();
}

vector<pair<String, String>> Feature::generateArffHeaderPortion() {
	vector<pair<String, String>> result;
    if(values.size() > 1) {
        for(int i = 0; i < values.size() ; i++) {
            result.push_back({String(name + "_" + to_string(i)), num});
        }
    } else {
        result.push_back({name, num});
    }
	return result;
}

vector<String> Feature::generateArffLinePortion() {
	vector<String> result;
	for (double value : values) {
		result.push_back(to_string(value));
	}
	return result;
}
