#include <math.h>
#include <sstream>
#include <windows.h>

#pragma once

#define M_PI       3.14159265358979323846

template <typename Type> class Array {
private:
	int _rowCount;
	int _colCount;
	Type *arr;
	Type null;
public:
	Array(int colCount, int rowCount) {
		_rowCount = rowCount;
		_colCount = colCount;
		arr = new Type[rowCount*colCount];
	};
	~Array() {
		delete[] arr;
	};
	Type &operator[](int i) {return arr[i];};
	Type &operator()(int x, int y) {
		try {
			if (x>=0 && x<_colCount && y>=0 && y<_rowCount)
				return arr[x + y*_colCount];
			else
				throw 1;
		} catch(int i) {
			MessageBox(NULL, (Convert(x) + ", " + Convert(y)).c_str(), "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
		}
	};
	int getW() {return _colCount;};
	int getH() {return _rowCount;};
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
	Point <int> round() {
		Point <int> result;
		result.x=(int) floor(x + 0.5);
		result.y=(int) floor(y + 0.5);
		return result;
	}
};

template <typename T> class Vector: public Point <T> {
public:
	Vector(T _x, T _y) : Point(_x, _y) {};
	Vector() : Point() {};
	Vector(Point <T> p) : Point(p) {};

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
	bool operator==(Vector v) {
		if (x == v.x && y == v.y) return true;
		else return false;
	};
	bool operator!=(Vector v) {
		if (x == v.x && y == v.y) return false;
		else return true;
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
	void setAngle(double angle) {
		double length = getLength();
		fromDeg(angle);
		x*=length;
		y*=length;
	};
	void multiplying(double a11,double a12,double a21,double a22) {
		Vector <T> result;
		result.x = x * a11 + y * a21;
		result.y = x * a12 + y * a22;
		*this = result;
	};
	void rotate(double angle) {
		Vector <T> result;
		double cost = cos(angle), sint = sin(angle);
		result.x = x * cost - y * sint;
		result.y = x * sint + y * cost;
		*this = result;
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
	double getDot(Vector v) {
		if (getLength()==0 || v.getLength()==0) return 0;
		return acos((x*v.x + y*v.y)/sqrt((x*x+y*y)*(v.x*v.x+v.y*v.y)));
	}
};

template <typename T>
std::string Convert (T number) {
	std::ostringstream buff;
	buff << number;
	return buff.str();
}
