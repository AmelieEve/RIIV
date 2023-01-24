#ifndef CREATION_ARFF
#define CREATION_ARFF

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

String generateARFFHeader(String name, list<pair<String, String>> attributes);
String generateARFFLine(list<String> attributes);

#endif // !CREATION_ARFF
