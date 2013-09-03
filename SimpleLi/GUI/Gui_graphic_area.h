#include "Main_inclusion.h"
#include "Gui_color.h"

template <typename Type> class Array {
private:
	int rowCount;
	int colCount;
	Type *arr;
	Type null;
public:
	Array(int colCount, int rowCount) {
		arr = new Type[rowCount*colCount];
	};
	Type *operator()(int x, int y) {
		return &arr[x + y*colCount];
	};
};

struct Cell {
private:
	hgeQuad q;
public:
	Cell() {};
	Cell(int x, int y, int w, int h, DWORD color) {
		q.v[0].x=x;
		q.v[0].y=y;
		q.v[1].x=x+w;
		q.v[1].y=y;
		q.v[2].x=x+w;
		q.v[2].y=y+h;
		q.v[3].x=x;
		q.v[3].y=y+h;
	};

	void setQuad(int x, int y, int w, int h, DWORD color) {
		*this = Cell(x, y, w, h, color);
	};

	void setQuadColor(DWORD color) {
		q.v[0].col=
		q.v[1].col=
		q.v[2].col=
		q.v[3].col=color;
	};
};

class GraphicArea {
private:
	int width;
	int height;
	
	int screenW;
	int screenH;
	
	int visibleX, visibleY;
	int visibleW, visibleH;
	Array <Cell> quadArr;
public:
	GraphicArea(int w, int h):
	quadArr(w, h) {
		for(int x=0; x<w; x++)
			for(int y=0; y<h; y++) 
				quadArr(x, y)->setQuad(x, y, w, h, 0xFFFFFFFF);
	};
	void setVisibleArea(int x, int y, int w, int h) {
		x<=0 ? visibleX = 0 : visibleX = x;
		x>=width ? visibleX = width-1 : visibleX = x;
		y<=0 ? visibleY = 0 : visibleY = y;
		y>=height ? visibleY = height-1 : visibleY = y;
	};
	DWORD *operator()(int x, int y) {};
	
	void Render() {};
};