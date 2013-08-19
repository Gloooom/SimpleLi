
#pragma once

#include "..\Logic\Types.h"

namespace func {

	bool randBool();

	float randf(float start, float end);

	long long int randi(long long int start, long long int end);

	bool sign(double value);

	long long int round(double value);

	Point <int> round(Point <float> P);

	Point <double> crossLine(double k1, double b1, double k2, double b2);

	template <typename F>
	F getVariation (F d, double coef) {
		//coef определяет максимально возможное отклонение в процентах.
		return (d + d*(((((double) rand())/(RAND_MAX/2)) - 1) * coef));
	}

	template <typename Type>
	bool isIn(std::vector <Type> v, Type that) {
		return std::find(v.begin(), v.end(), that) != v.end();
	}
}