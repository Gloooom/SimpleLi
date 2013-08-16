#include "..\stdafx.h"

#pragma once

#define W 25
#define H 25
#define POP_A 10
#define POP_G 0

#define _USE_MATH_DEFINES 
#include <math.h>


enum Mode_hibrid {HALF_TO_HALF, AVERAGE};
enum Mode_change_what {SOC, SOC_PHIS, PHIS};
enum Mode_mutation {ONE, ALL, HALF};
enum Mode_feeding {AUTO, GETERO};
enum Type_eye {RADIUS, TRIANGLE, SECTOR};

enum Individ_status {HUNGRY, EAT, MATURE, REPRODUCT, WAIT, end_of_status};


struct FOV {
	Type_eye type;
	float angle;
	float height;
	float width;
};

template <typename Type> class IndMemory {
public:
	std::vector <Type> enemies;
	std::vector <Type> partners;
	void clear() {
		enemies.clear();
		partners.clear();
	};
	void operator=(IndMemory m) { 
		enemies = m.enemies;
		partners = m.partners;
	};
};

template <typename cellType> class row {
public:
	int colCount;
	cellType *cells;
public:
	row() {
		colCount=1;
		cells = new cellType;
	};
	row(int c) {
		colCount=c;
		cells = new cellType[c];
	};
	~row() {
		delete [] cells;
	};
	void init(int c) {
		colCount=c;
		cells = new cellType[c];
	}
	cellType &operator[](int c) {
		if (c>=0 && c<colCount)
			return cells[c];
		else return cells[0];
	};
};

template <typename Type> class Mass {
public:
	int rowCount;
	row <Type> *rows;
public:
	Mass(int r, int c) {
		rowCount=r;
		rows = new row<Type>[r];
		for (int i=0; i<r; i++) {
			rows[i].init(c);
		}
	};
	~Mass() {
		delete [] rows;
	};
	row <Type> &operator[](int r) {
		if (r>=0 && r<rowCount)
			return rows[r];
		else return rows[0];
	};
};


template <typename T> class Point {
public:
	T x;
	T y;
public:
	Point() {
		x=0;
		y=0;
	};
	Point(T _x, T _y) {
		x=_x;
		y=_y;
	};
	bool operator==(Point p) {
		if (x==p.x && y==p.y) return true;
		else return false;
	};
	void operator=(Point p) {
		x=p.x;
		y=p.y;
	};
	void toPolar(double *a, double *h) {
		*h=sqrt(x*x+y*y);
		Vector v = *this;
		*a=v.getDeg();
	};
};

template <typename T> class Vector: public Point <T> {
public:
	Vector(T _x, T _y) : Point(_x, _y) {};
	Vector() : Point() {};
	Vector(Point <T> p) : Point(p) {};

	//using Point::operator=;
	void operator=(Point <T> p) {
		x=p.x;
		y=p.y;
	};
	Vector operator-(Vector v) const{
		Vector <T> result;
		result.x=x-v.x;
		result.y=y-v.y;
		return result;
	};
	Vector operator+(Vector v) const{
		Vector <T> result;
		result.x=x+v.x;
		result.y=y+v.y;
		return result;
	};
	void operator-=(Vector v) {
		(*this)=(*this)-v;
	};
	void operator+=(Vector v) {
		(*this)=(*this)+v; 
	};
	Vector operator*(T p) const {
		Vector <T> result;
		result.x=x*p;
		result.y=y*p;
		return result;
	};
	Vector operator/(T P) {
		Vector <T> result;
		result.x=x/P;
		result.y=y/P;
		return result;
	};
	bool operator<(Vector v) {
		if (this->getLength()<v.getLength()) return true;
		else return false;
	};
	bool operator>(Vector v) {
		if (this->getLength()>v.getLength()) return true;
		else return false;
	};
	bool operator<=(Vector v) {
		if (this->getLength()<=v.getLength()) return true;
		else return false;
	};
	bool operator>=(Vector v) {
		if (this->getLength()>=v.getLength()) return true;
		else return false;
	};
	Vector getNorm() const {
		Vector <T> result = *this;
		if (x!=0 || y!=0) {
			double lenRcp = 1/sqrt(x*x+y*y);
			result.x *= lenRcp;
			result.y *= lenRcp;
		} else {
			result.x=0;
			result.y=0;
		}
		return result;
	};

	double getAngle(Vector v) const {
		if (x!=0 || y!=0)
			return acos((x*v.x+y*v.y)/(sqrt(x*x+y*y)*sqrt(v.x*v.x+v.y*v.y)));
		else return 0;
	};

	double getDeg() const {
		double angle;
		if (x!=0 || y!=0) {
			if (y<0)
				angle=(2*M_PI)-acos(x/sqrt(x*x+y*y));
			else
				angle=acos(x/sqrt(x*x+y*y));
			return angle;
		} else return 0;
	};

	void fromDeg(double angle) {
		x = cos(angle);
		y = sin(angle);
	};

	double getLength() {
		return sqrt((double)(x*x+y*y));
	};
};




