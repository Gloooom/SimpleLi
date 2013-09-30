#include <math.h>
#include <sstream>
#include <windows.h>

#pragma once

#define M_PI       3.14159265358979323846

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


	Vector <int> toInt() {
		Vector <int> v;
		v.x = (int)x;
		v.y = (int)y;
		return v;
	};
	Vector <double> toDouble() {
		Vector <double> v;
		v.x = (double)x;
		v.y = (double)y;
		return v;
	};
	Vector <int> round() {
		Vector <int> v;
		v.x = func::round(x);
		v.y = func::round(y);
		return v;
	};
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
		Vector <T> result = *this;
		if (P != 0) {
			result.x=x/P;
			result.y=y/P;
		}
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
		if (!(x == 0 && y == 0)) {
			double lenRcp = 1/sqrt(x*x+y*y);
			result.x *= lenRcp; //Делю на длину вектора и получаю нормированный вектор.
			result.y *= lenRcp;
		} else {
			result.x=0;
			result.y=0;
		}
		return result;
	};
	double getAngle(Vector v) const {
		if ((x == 0 && y == 0) || (v.x == 0 && v.y == 0)) 
			return 0;
		else
			return acos((x*v.x+y*v.y)/(sqrt(x*x+y*y)*sqrt(v.x*v.x+v.y*v.y)));
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
		if (x == 0 && y == 0) 
			return 0;
		else {
			if (y<0)
				return (2*M_PI)-acos(x/sqrt(x*x+y*y));
			else
				return acos(x/sqrt(x*x+y*y));
		}
	};
	void fromDeg(double angle) {
		x = cos(angle);
		y = sin(angle);
	};
	double getLength() {
		if (x == 0 && y == 0) return 0;
		return sqrt((double)(x*x+y*y));
	};
	double getDot(Vector v) {
		if ((x == 0 && y == 0) || (v.x == 0 && v.y == 0)) 
			return 0;
		else 
			return acos((x*v.x + y*v.y)/sqrt((x*x+y*y)*(v.x*v.x+v.y*v.y)));
	}
};

template <typename T>
std::string Convert (T number) {
	std::ostringstream buff;
	buff << number;
	return buff.str();
}

		
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
	Type &operator()(Point <int> pos) {
		return (*this)(pos.x, pos.y);
	};
	int getW() {return _colCount;};
	int getH() {return _rowCount;};

	bool isValid(Point <int> pos) {
		if (pos.x >= 0 && pos.x <_colCount && 
			pos.y >= 0 && pos.y <_rowCount)
			return true;
		else return false;
	};

	void fill(Type obj) {
		Type *addrBuffer = arr;
		Type *endBuffer = addrBuffer + (_colCount * _rowCount-1);
		for (; addrBuffer <= endBuffer; addrBuffer++) 
			*addrBuffer = obj;
	};
};

//
//class Error {
//private:
//	static Error *_self;
//	Error() {};
//	~Error() {};
//public:
//	static void throwError(std::string err) {
//
//	}
//};




