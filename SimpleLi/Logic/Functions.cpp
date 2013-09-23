#include "inclusion.h"

#pragma once

#include "Functions.h"


bool func::randBool() {
	return (bool)(rand()/(RAND_MAX/2));
}

float func::randf(float start, float end) {
	return (((float) rand())/RAND_MAX)*(end-start) + start;
}

long long int func::randi(long long int start, long long int end) {
	end++;
	return randf(start, ((float)end)-0.00001);
}

bool func::randPercentBool(float percent) {
	if (randf(0, 1) <= percent) return true;
	else return false;
}

bool func::sign(double value) {
	if (value<0) return 0;
	else return 1;
}

Point <double> func::crossLine(double k1, double b1, double k2, double b2) {
	Point <double> cross;
	if (k1 == k2) return cross;
	cross.x=(b2-b1)/(k1-k2);
	cross.y=k1*cross.x+b1;
	return cross;
}

long long int func::round(double value) {
	return (long long int) floor(value + 0.5);
}
