#include "shape.h"

/*
descr:
	vec3 center = vec3(0);
	float r = 0.f;
	vec3 N = vec3(0);
	float D = 0.f;
	vec3 sides = vec3(1);
	float R = 0.f;
	vec3 cap = vec3(0);
	float padding;
*/

string descrToString(uint type, descr& D) {
	stringstream ss;
	if (type == sindexes::SPHERE || type == sindexes::TORUS) {
		ss << D.center.x << ',' << D.center.y << ',' << D.center.z << ',';
		ss << D.r << ',';
		if (type == sindexes::TORUS) ss << D.R << ',';
	}
	if (type == sindexes::PLANE) {
		ss << D.N.x << ',' << D.N.y << ',' << D.N.z << ',';
		ss << D.D << ',';
	}
	if (type == sindexes::BOX || type == sindexes::ELLIPSOID) {
		ss << D.center.x << ',' << D.center.y << ',' << D.center.z << ',';
		ss << D.sides.x << ',' << D.sides.y << ',' << D.sides.z << ',';
	}
	if (type == sindexes::CYLINDER) {
		ss << D.center.x << ',' << D.center.y << ',' << D.center.z << ',';
		ss << D.cap.x << ',' << D.cap.y << ',' << D.cap.z << ',';
	}
	return ss.str();
}


descr stringToDescr(uint type, string str) {
	descr D;
	size_t start = 0, end = str.find(",");

	if (type == sindexes::SPHERE || type == sindexes::TORUS) {
		D.center.x = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);
		D.center.y = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);
		D.center.z = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);

		D.r = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);
		if (type == sindexes::TORUS) D.R = stof(str.substr(start, end - start));
	}
	if (type == sindexes::PLANE) {
		D.N.x = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);
		D.N.y = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);
		D.N.z = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);

		D.D = stof(str.substr(start, end));
	}
	if (type == sindexes::BOX || type == sindexes::ELLIPSOID) {
		D.center.x = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);
		D.center.y = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);
		D.center.z = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);

		D.sides.x = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);
		D.sides.y = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);
		D.sides.z = stof(str.substr(start, end - start));
	}
	if (type == sindexes::CYLINDER) {
		D.center.x = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);
		D.center.y = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);
		D.center.z = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);

		D.cap.x = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);
		D.cap.y = stof(str.substr(start, end - start)), start = end + 1, end = str.find(",", start);
		D.cap.z = stof(str.substr(start, end - start));
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