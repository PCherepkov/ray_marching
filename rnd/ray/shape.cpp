#include "shape.h"

string descrToString(uint type, descr& D) {
	stringstream ss;
	if (type == sindexes::SPHERE) {
		ss << D.center.x << ',' << D.center.y << ',' << D.center.z << ',';
		ss << D.r << ',';
	}
	if (type == sindexes::PLANE) {
		ss << D.N.x << ',' << D.N.y << ',' << D.N.z << ',';
		ss << D.D << ',';
	}
	return ss.str();
}


descr stringToDescr(uint type, string str) {
	descr D;
	size_t start = 0, end = str.find(",");

	if (type == sindexes::SPHERE) {
		D.center.x = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);
		D.center.y = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);
		D.center.z = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);

		D.r = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);
	}
	if (type == sindexes::PLANE) {
		D.N.x = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);
		D.N.y = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);
		D.N.z = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);;

		D.D = stof(str.substr(start, end)), start = end + 1, end = str.find(",", start);
	}
	return D;
}


/*
if (type == sindexes::SPHERE) {
		D.center.x = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);
		D.center.y = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);
		D.center.z = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);

		D.r = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);
	}
	if (type == sindexes::PLANE) {
		D.N.x = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);
		D.N.y = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);
		D.N.z = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);;

		D.D = stof(str.substr(start, end)), start = end + 1, end = str.find(",", start);
	}
*/