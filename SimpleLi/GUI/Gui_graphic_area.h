#include "Gui_color.h"

extern HGE *hge;

struct Cell {
private:
	hgeQuad q;
public:
	Cell() {
		*this = Cell(0, 0, 1, 1);
	};
	Cell(int x, int y, int w, int h) {
		setQuad(x, y, w, h);
		setColor(0xFFFFFFFF);
		q.v[0].z=
			q.v[1].z=
			q.v[2].z=
			q.v[3].z=0.5f;
		q.blend=BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE;
		q.tex=0;
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
	Point <double> getCenterPos() {
		Point <double> result;
		result.x = (q.v[0].x + q.v[2].x)/2;
		result.y = (q.v[0].y + q.v[2].y)/2;
		return result;
	}
	hgeQuad *getQuad() {return &q;};
};

class GraphicArea {
private:
	float _border;

	int _screenWidth;
	int _screenHeight;

	int _colCount;
	int _rowCount;

	float _visibleX, _visibleY;
	float _visibleColCount, _visibleRowCount;

	float _cellWidth;
	float _cellHeight;

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
			setVisibleArea(0, 0, _colCount, _rowCount);
	};
	  
	Cell operator[](int i) {return quadArr[i];};
	DWORD &operator()(int x, int y) {return colorArr(x, y);};
	DWORD &operator()(Point <int> p) {return colorArr(p); };

	void setBorder(int b) {
		  _border = b;
		  setVisibleArea(_visibleX, _visibleY, _visibleColCount, _visibleRowCount);
	};

	void Update() {
		  for(int x=_visibleX; x<_visibleColCount+_visibleX; x++)
			  for(int y=_visibleY; y<_visibleRowCount+_visibleY; y++)
				  quadArr(x, y).setColor(colorArr(x, y));
	};
	void Render() {
		for(int x=_visibleX; x<_visibleColCount+_visibleX; x++)
			for(int y=_visibleY; y<_visibleRowCount+_visibleY; y++)
				hge->Gfx_RenderQuad(quadArr(x, y).getQuad());
	};
	void setBackground(DWORD color) {
		_background = color;
	};
	void Clear() {
		for(int x=_visibleX; x<_visibleColCount+_visibleX; x++)
			for(int y=_visibleY; y<_visibleRowCount+_visibleY; y++)
				colorArr(x, y) = _background;
	};
	bool getMousePos(int mpx, int mpy, int *colPos, int *rowPos) {
		if (mpx<_screenWidth && mpy<_screenHeight) {
			*colPos = (mpx/(_cellWidth+_border))+_visibleX;
			*rowPos = (mpy/(_cellHeight+_border))+_visibleY;
			return true;
		} else {
			*colPos = -1;
			*rowPos = -1;
			return false;
		}
	};
	bool checkVisiblity(int x, int y) {
		if (x>=_visibleX && y>=_visibleY &&
			x<_visibleColCount + _visibleX &&
			y<_visibleRowCount + _visibleY) 
			return true;
		return false;
	};

	void RenderInfo(std::vector <std::vector <Vector <int>>> *polygons) {
		//Дописать в плане масштабирования
		hgeTriple _poly;
		_poly.v[0].col = 
		_poly.v[1].col = 
		_poly.v[2].col = 0x44FF0000;
		_poly.v[0].z = 
		_poly.v[1].z = 
		_poly.v[2].z = 0.5;
		_poly.tex = NULL;
		_poly.blend = BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE;

		std::vector <std::vector <Vector <int>>> ::iterator p = polygons->begin();
		while (p != polygons->end()) {	
			//Расчёт и рендер областей зрения.
			Vector <double> vert;
			for (int i=0; i<3; i++) {
				_poly.v[i].x = (*p)[i].x*(_cellWidth+_border)+_cellWidth/2;
				_poly.v[i].y = (*p)[i].y*(_cellHeight+_border)+_cellHeight/2;
			}
			hge->Gfx_RenderTriple(&_poly);
			p++;
		}	
	};

	int getColCount() {	return _colCount;};
	int getRowCount() { return _rowCount;};

	int getCellWidth() {return _cellWidth;};
	int getCellHeight() {return _cellHeight;};

	int getVisibleX() {return _visibleX;};
	int getVisibleY() {return _visibleY;};
	
	void setZoom(float coef) {
		coef*=10;
		coef = (int) coef;
		coef/=10;
		int tempX = _visibleX;
		int tempY = _visibleY;
		int tempVisibleCol = _visibleColCount;
		int tempVisibleRow = _visibleRowCount;

		int visibleCol = _colCount/coef;
		int visibleRow = _rowCount/coef;
		int visibleX = 0;
		int visibleY = 0;
		if (visibleCol % 2 != 0) visibleCol++;
		if (visibleRow % 2 != 0) visibleRow++;

		if (visibleCol <= _colCount && visibleRow <=_rowCount)
			setVisibleArea(visibleX, visibleY, visibleCol, visibleRow);
		setPos(
			(tempX*2+tempVisibleCol)/2 - visibleCol/2, 
			(tempY*2+tempVisibleRow)/2 - visibleRow/2
			);
	};

	void setPos(int x, int y) {
		if (x<=0) x = 0;
		else if (x>_colCount-_visibleColCount) x = _colCount-_visibleColCount;
		if (y<=0) y = 0;
		else if (y>_rowCount-_visibleRowCount) y = _rowCount-_visibleRowCount;
			setVisibleArea(x, y, _visibleColCount, _visibleRowCount);
	};

private:
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

		  _cellWidth = (float)_screenWidth/(float)visibleColCount - _border;
		  _cellHeight = (float)_screenHeight/(float)visibleRowCount - _border;
		  for(int _x=visibleX; _x<visibleColCount+visibleX; _x++)
			  for(int _y=visibleY; _y<visibleRowCount+visibleY; _y++)
				  quadArr(_x, _y).setQuad(_cellWidth*(_x-visibleX)+(_border*(_x-visibleX)),
				  _cellHeight*(_y-visibleY)+(_border*(_y-visibleY)), 
				  _cellWidth, _cellHeight);
	};
};
