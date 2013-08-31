#include "stdafx.h"

#include "..\HGE\hge.h"

#pragma once

extern HGE *hge;

float isRight(float that);

class Pixel {
private:
	HTEXTURE pix;
public:
	Pixel(DWORD color) {
		pix = hge->Texture_Create(1, 1);
		setCol(color);
	};
	Pixel() {
		pix = hge->Texture_Create(1, 1);
		setCol(0xFFFFFFFF);
	};
	~Pixel() {
		hge->Texture_Free(pix);
	};

	HTEXTURE getTex() {return pix;};
	operator HTEXTURE() {return pix;};

	void	 setCol(DWORD color) {
		DWORD *ptr = hge->Texture_Lock(pix, false, 0, 0, 1, 1);
		ptr[0] = color;
		hge->Texture_Unlock(pix);
	};
};

class HSVColor;
class RGBColor {
private:
	float a, r, g, b;
public:
	RGBColor(): 
	a(0), r(0), g(0), b(0) {};
	RGBColor(float _r, float _g, float _b): r(_r), g(_g), b(_b) {};
	RGBColor(DWORD color);
	RGBColor(HSVColor color);

	float getA() {return a;};
	float getR() {return r;};
	float getG() {return g;};
	float getB() {return b;};

	void setA(float _A) {a = isRight(_A);};
	void setR(float _R) {r = isRight(_R);};
	void setG(float _G) {g = isRight(_G);};
	void setB(float _B) {b = isRight(_B);};

	DWORD getDWORD();
	operator DWORD() {return getDWORD();};
};

class HSVColor {
private:
	float h, s, v;
private:
	float getMin(float a, float b) {return a<b ? a : b;};
	float getMax(float a, float b) {return a>b ? a : b;};
public:	
	HSVColor():
	h(0), s(0), v(0) {};
	HSVColor(float _h, float _s, float _v): h(_h), s(_s), v(_v) {};
	HSVColor(DWORD color) {*this = HSVColor(RGBColor(color));};
	HSVColor(RGBColor color);

	float getHue() {return h;};
	float getSaturation() {return s;};
	float getValue() {return v;};

	HSVColor setHue(float _H) {
		h = _H;
		if (_H<=0) h = 0;
		if (_H>=360) h = 359.9f;
		return *this;
	};
	HSVColor setSaturation(float _S) {s = isRight(_S); return *this;};
	HSVColor setValue(float _V) {v = isRight(_V); return *this;};

	operator DWORD() {return RGBColor(*this);};
};
