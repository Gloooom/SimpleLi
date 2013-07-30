
#pragma once

#include "..\Logic\Types.h"

namespace func {

	bool randBool();

	float randf(float start, float end);

	bool sign(double value);

	long long int round(double value);

	Point <int> round(Point <float> P);

	Point <double> crossLine(double k1, double b1, double k2, double b2);

	template <typename F>
	F getVariation (F d, double coef) {
		return (d + d*(((((double) rand())/(RAND_MAX/2)) - 1) * coef));
	}

	template <typename Type>
	bool isIn(std::vector <Type> v, Type that) {
		std::vector<Type>::iterator p = v.begin();
		while (p != v.end()) {
			if (*p == that) return true;
			p++;
		}
		return false;
	}
}