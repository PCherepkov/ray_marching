#include "shape.h"

string descrToString(descr& D) {
	stringstream ss;
	ss << D.center.x << ',' << D.center.y << ',' << D.center.z << ',';
	ss << D.r << ',';
	ss << D.N.x << ',' << D.N.y << ',' << D.N.z << ',';
	ss << D.D;
	return ss.str();
}


descr stringToDescr(string str) {
	descr D;
	size_t start = 0, end = str.find(",");
	D.center.x = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);
	D.center.y = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);
	D.center.z = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);

	D.r = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);

	D.N.x = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);
	D.N.y = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);
	D.N.z = stof(str.substr(start, end - start)), start = end + 1, end = string::npos;

	D.D = stof(str.substr(start, end));
	return D;
}
