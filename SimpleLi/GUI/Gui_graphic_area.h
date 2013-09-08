#include "Gui_color.h"

extern HGE *hge;

struct Cell {
private:
	hgeQuad q;
public:
	Cell() {};
	Cell(int x, int y, int w, int h) {
		setQuad(x, y, w, h);
		setColor(0xFFFFFFFF);
	};

	void setQuad(int x, int y, int w, int h) {
		q.v[0].x=x;
		q.v[0].y=y;
		q.v[1].x=x+w;
		q.v[1].y=y;
		q.v[2].x=x+w;
		q.v[2].y=y+h;
		q.v[3].x=x;
		q.v[3].y=y+h;
	};
	void setColor(DWORD color) {
		q.v[0].col=
			q.v[1].col=
			q.v[2].col=
			q.v[3].col=color;
	};
	hgeQuad *getQuad() {return &q;};
};

class GraphicArea {
private:
	int _border;

	int _screenWidth;
	int _screenHeight;

	int _colCount;
	int _rowCount;

	int _visibleX, _visibleY;
	int _visibleColCount, _visibleRowCount;

	int _cellWidth;
	int _cellHeight;

	DWORD _background;
	Array <DWORD> colorArr;
	Array <Cell> quadArr;
public:
	GraphicArea(int colCount, int rowCount, int screenWidth, int screenHeight):
	  quadArr(colCount, rowCount),
		  colorArr(colCount, rowCount) {
			  _visibleX = 0; _visibleY = 0;
			  _visibleColCount = colCount;
			  _visibleRowCount = rowCount;
			  _border = 0;
			  _colCount = colCount;
			  _rowCount = rowCount;
			  _screenWidth = screenWidth;
			  _screenHeight = screenHeight;
			  _background = 0xFFFFFFFF;
			  for(int x=0; x<colCount; x++)
				  for(int y=0; y<rowCount; y++) {
					  quadArr(x, y).setQuad(0, 0, 1, 1);
					  colorArr(x, y) = 0xFFFFFFFF;
				  }
	};
	void setBorder(int b) {
		  _border = b;
		  setVisibleArea(_visibleX, _visibleY, _visibleColCount, _visibleRowCount);
	};
	void setVisibleArea(int visibleX, int visibleY, int visibleColCount, int visibleRowCount) {
		  if (visibleX<=0) _visibleX = 0;
		  if (visibleX>=_colCount) visibleX = _colCount;
		  if (visibleY<=0) visibleY = 0;
		  if (visibleY>=_rowCount) visibleY = _rowCount;
		  _visibleY = visibleY;
		  _visibleX = visibleX;

		  if (visibleColCount<=0) visibleColCount = 1;
		  if (visibleRowCount<=0) visibleRowCount = 1;

		  _visibleColCount = visibleColCount;
		  _visibleRowCount = visibleRowCount;

		  _cellWidth = _screenWidth/_visibleColCount - _border;
		  _cellHeight = _screenHeight/_visibleRowCount - _border;
		  for(int _x=visibleX; _x<visibleColCount; _x++)
			  for(int _y=visibleY; _y<visibleRowCount; _y++)
				  quadArr(_x, _y).setQuad((_x-visibleX)*_cellWidth+(_border*(_x-visibleX)),
				  (_y-visibleY)*_cellHeight+(_border*(_y-visibleY)), 
				  _cellWidth, _cellHeight);
	};
	Cell operator[](int i) {return quadArr[i];};
	DWORD &operator()(int x, int y) {return colorArr(x, y);};
	void Update() {
		  for(int x=_visibleX; x<_visibleColCount; x++)
			  for(int y=_visibleY; y<_visibleRowCount; y++)
				  quadArr(x, y).setColor(colorArr(x, y));
	};
	void Render() {
		  for(int x=_visibleX; x<_visibleColCount; x++)
			  for(int y=_visibleY; y<_visibleRowCount; y++)
				  hge->Gfx_RenderQuad(quadArr(x, y).getQuad());
	};
	void setBackground(DWORD color) {
		  _background = color;
	};
	void Clear() {
		  for(int x=_visibleX; x<_visibleColCount; x++)
			  for(int y=_visibleY; y<_visibleRowCount; y++)
				  colorArr(x, y) = _background;
	};
	void getMousePos(int mpx, int mpy, int *colPos, int *rowPos) {
		if (mpx<_screenWidth && mpy<_screenHeight) {
			*colPos = mpx/(_cellWidth+_border);
			*rowPos = mpy/(_cellHeight+_border);
		} else {
			*colPos = _visibleColCount-1;
			*rowPos = _visibleRowCount-1;
		}
	};
};
