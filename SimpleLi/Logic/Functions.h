#include "Types.h"

#pragma once

namespace func {

	bool randBool();

	float randf(float start, float end);

	long long int randi(long long int start, long long int end);

	bool sign(double value);

	long long int round(double value);

	bool randPercentBool(float percent);

	Point <double> crossLine(double k1, double b1, double k2, double b2);

	template <typename T>
	T getMin(T a, T b) {return a<b ? a : b;};
	template <typename T>
	T getMax(T a, T b) {return a>b ? a : b;};

	template <typename F>
	F getVariation (F d, double coef, bool negativeValue = true) {
		//coef определяет максимально возможное отклонение в процентах.
		if (negativeValue) {
			if (d!=0) return (d + (d*(((((double) rand())/(RAND_MAX/2)) - 1) * coef)));
			if (d==0) return (((((double) rand())/(RAND_MAX/2)) - 1) * coef);
		} else {
			d = (d + d*(((((double) rand())/(RAND_MAX/2)) - 1) * coef));
			if (d<=0) return 0;
			return d;
		}
		return 0;
	}

	template <typename Type>
	bool isIn(std::vector <Type> v, Type that) {
		return std::find(v.begin(), v.end(), that) != v.end();
	}
}