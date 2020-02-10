#include "utility.hpp"

#include <cmath>
#include <cstdlib>

void split(const std::string& str, char delm, std::vector<std::string>& strs) {
	size_t begin = 0, i;
	std::string aux;

	strs.clear();

	while ((i = str.find_first_of(delm, begin)) != std::string::npos) {
		if (i == begin) {
			begin++;
			continue;
		}

		aux = str.substr(begin, i - begin);
		begin = i + 1;

		strs.push_back(aux);
	}
	if (begin < str.size())
		strs.push_back(str.substr(begin, str.size() - begin));
}

float distance(Coord p1, Coord p2) {
	float dx = p1.x - p2.x;
	float dy = p1.y - p2.y;
	float dz = p1.z - p2.z;

	return sqrtf(dx*dx + dy*dy + dz*dz);
}

float my_stof(const std::string& str){
    return atof(str.c_str());
}

unsigned long my_stoul(const std::string& str){
    return atol(str.c_str());
}
