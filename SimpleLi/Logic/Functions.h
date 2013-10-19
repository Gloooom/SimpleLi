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

	template <typename F>
	F getVariation (F def, Interval interval, float coef, bool zeroValue = true) {
		/*
		def			- значение, измененную версию которого нужно получить
		interval	- определяет границы интервала, в котором нужно получить значение
		coef		- определяет максимально возможное отклонение в процентах 
		диапазон отклонения равен проценту, взятому от длины интервала.
		максимальное отклонение от def равно половине диапазоны; т.е. */
		F maxDelta = (abs(interval.end - interval.start)*coef)/2;

		F result = def + randf(maxDelta*(-1), maxDelta);

		if (result <= interval.start) result = interval.start;
		if (result >= interval.end) result = interval.end;
		if (result == 0 && !zeroValue) result = 0.01;
		return result;
	}

	template <typename Type>
	bool isIn(std::vector <Type> v, Type that) {
		return std::find(v.begin(), v.end(), that) != v.end();
	}
}