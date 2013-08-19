#include "..\stdafx.h"

#pragma once

#include "..\Logic\Functions.h"

bool func::randBool() {
	return (rand()/(RAND_MAX/2));
}

float func::randf(float start, float end) {
	return ((((float) rand())/(RAND_MAX/(end-start))) + start);
}

long long int func::randi(long long int start, long long int end) {
	end++;
	return ((((float) rand())/(RAND_MAX/(end-start))) + start);
}

bool func::sign(double value) {
	if (value<0) return 0;
	else return 1;
}

Point <double> func::crossLine(double k1, double b1, double k2, double b2) {
	Point <double> cross;
	cross.x=(b2-b1)/(k1-k2);
	cross.y=k1*cross.x+b1;
	return cross;
}

long long int func::round(double value) {
	return (long long int) floor(value + 0.5);
}

Point <int> func::round(Point <float> P) {
	Vector <int> result;
	result.x=(int) floor(P.x + 0.5);
	result.y=(int) floor(P.y + 0.5);
	return result;
}