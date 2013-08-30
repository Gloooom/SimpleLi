#include "Gui_color.h"

RGBColor::RGBColor(DWORD color) {
	enum {B, G, R, A};
	union {
		DWORD dw;
		unsigned char c[4];
	} tempCol;
	tempCol.dw = color;
	a = tempCol.c[A]/255.0f;
	r = tempCol.c[R]/255.0f;
	g = tempCol.c[G]/255.0f;
	b = tempCol.c[B]/255.0f;
};

RGBColor::RGBColor(HSVColor color) {
	float h = color.getHue();
	float s = color.getSaturation();
	float v = color.getValue();

	int Hi = h/60;
	if (Hi == 6) Hi = 5;
	float Vmin = (1-s)*v;
	float A = (v-Vmin)*(((float)((int)h % 60))/60);
	float Vinc = Vmin + A;
	float Vdec = v - A;
	
	a = 1;

	switch (Hi) {
	case 0: 
		r = v; g = Vinc; b = Vmin;
		break;
	case 1: 
		r = Vdec; g = v; b = Vmin;
		break;
	case 2: 
		r = Vmin; g = v; b = Vinc;
		break;
	case 3: 
		r = Vmin; g = Vdec; b = v;
		break;
	case 4: 
		r = Vinc; g = Vmin;	b = v;
		break;
	case 5: 
		r = v; g = Vmin; b = Vdec;
		break;
	}
};

DWORD RGBColor::getDWORD() {
	enum {B, G, R, A};
	union {
		DWORD dw;
		unsigned char c[4];
	} tempCol;

	tempCol.c[A] = a*255.0f;
	tempCol.c[R] = r*255.0f;
	tempCol.c[G] = g*255.0f;
	tempCol.c[B] = b*255.0f;
	return tempCol.dw;
};

HSVColor::HSVColor(RGBColor color) {
	float r = color.getR();
	float g = color.getG();
	float b = color.getB();
	float min, max;
	min = getMin(r, getMin(g, b));
	max = getMax(r, getMax(g, b));
	if (min == max) 
		h = 0;
	else if (max == r && g >= b) 
		h = 60.0f*((g-b)/(max-min)) + 0;
	else if (max == r && g <  b) 
		h = 60.0f*((g-b)/(max-min)) + 360;
	else if (max == g) 
		h = 60.0f*((b-r)/(max-min)) + 120;
	else if (max == b) 
		h = 60.0f*((r-g)/(max-min)) + 240;

	if (max == 0) s = 0;
	else s = 1 - (min/max);

	v = max;
};

float isRight(float that) {
	if (that<=0) return 0;
	if (that>=1) return 1;
	return that;
}

float getValueFromY(float Y, float startY, float length, float startV, float endV) {
	return (((Y-startY)/length)*(endV-startV))+startV;
}

HTEXTURE getButtonTex(int w, int h, DWORD color) {
	HTEXTURE tex = hge->Texture_Create(w*2, h);
	DWORD *ptr = hge->Texture_Lock(tex, false, 0, 0, w*2, h);
	HSVColor tempCol(color);
	float uV = isRight(tempCol.getValue()-0.15);
	float dV = isRight(tempCol.getValue()+0.15);

	for (int x = 0; x<w*2; x++) 
		for (int y = 0; y<h; y++) {
			if (x == w-1 || x == w || ((x<w*2-1 && x>w-1) && y == 0) || ((x>0 && x<w-1) && y == h-1)) {
				ptr[y*w*2 + x] = tempCol.setValue(0.15);
				tempCol = color;
			} else if (x == 0 || x == w*2-1 || (x<w-1 && y == 0) || (x>w && y == h-1)) {
				ptr[y*w*2 + x] = tempCol.setValue(1);
				tempCol = color;
			} else if (x>w) {
				ptr[y*w*2 + x] = tempCol.setValue(getValueFromY(y,0,h,uV,dV));
				tempCol = color;
			} else { 
				ptr[y*w*2 + x] = tempCol.setValue(getValueFromY(y,0,h,dV,uV));
				tempCol = color;
			}
		}
		hge->Texture_Unlock(tex);
	return tex;
};