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
	Vector <double> getCenterPos() {
		Point <double> result;
		result.x = (q.v[0].x + q.v[2].x)/2;
		result.y = (q.v[0].y + q.v[2].y)/2;
		return result;
	}

	hgeQuad getOffsetQuad(float x, float y) {
		hgeQuad tempQ = q;
		tempQ.v[0].x+=x;
		tempQ.v[0].y+=y;
		tempQ.v[1].x+=x;
		tempQ.v[1].y+=y;
		tempQ.v[2].x+=x;
		tempQ.v[2].y+=y;
		tempQ.v[3].x+=x;
		tempQ.v[3].y+=y;
		return tempQ;
	}

	hgeQuad *getQuad() {return &q;};
};

class OldGraphicArea {
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
	OldGraphicArea(int colCount, int rowCount, int screenWidth, int screenHeight):
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
			*colPos = ((mpx+_border/4)/(_cellWidth+_border))+_visibleX;
			*rowPos = ((mpy+_border/4)/(_cellHeight+_border))+_visibleY;
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
				_poly.v[i].x = (*p)[i].x*(_cellWidth+_border+_border/2)+_cellWidth/2;
				_poly.v[i].y = (*p)[i].y*(_cellHeight+_border+_border/2)+_cellHeight/2;
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
		_border=(int) coef/2;
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

	void setPos(Vector <int> pos) {
		setPos(pos.x, pos.y);
	};
	void setPos(int x, int y) {
		if (x<=0) 
			x = 0;
		else if (x>_colCount-_visibleColCount) 
			x = _colCount-_visibleColCount;
		if (y<=0) 
			y = 0;
		else if (y>_rowCount-_visibleRowCount) 
			y = _rowCount-_visibleRowCount;
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
				  quadArr(_x, _y).setQuad(_cellWidth*(_x-visibleX)+(_border*(_x-visibleX))+_border/2,
				  _cellHeight*(_y-visibleY)+(_border*(_y-visibleY)) +_border/2, 
				  _cellWidth, _cellHeight);
	};
};
//////////////////////////////////////////
/////////////////////////////////////////
///////////////////////////////////////
/////////////////////////////////////
class GraphicArea {
private:
	int _border;

	int _screenWidth;
	int _screenHeight;

	
	Vector <int> _pos;
	int _totalW, _totalH;
	int _cellWidth;
	int _cellHeight;
	
	
	int _colCount;
	int _rowCount;

	int _visibleCol, _visibleRow; 
	int _visibleColCount, _visibleRowCount;

	float _zoom;

	DWORD _background;
	Array <DWORD> colorArr;
	Array <Cell> quadArr;
public:
	GraphicArea(int colCount, int rowCount, int screenWidth, int screenHeight):
		quadArr(colCount, rowCount),
		colorArr(colCount, rowCount) {
			_border = 0;
			_colCount = colCount;
			_rowCount = rowCount;
			_screenWidth = screenWidth;
			_screenHeight = screenHeight;
			_totalW = _screenWidth;
			_totalH = _screenHeight;
			_background = 0xFFFFFFFF;
			for(int x=0; x<colCount; x++)
				for(int y=0; y<rowCount; y++) {
					quadArr(x, y).setQuad(0, 0, 1, 1);
					colorArr(x, y) = 0xFFFFFFFF;
				}
			setZoom(1);
			int maxResolution = getMin(_screenWidth, _screenHeight);
			maxResolution /= 2;
			setCenterPos(maxResolution, maxResolution);
		};
	  
	Cell operator[](int i) {return quadArr[i];};
	DWORD &operator()(int x, int y) {return colorArr(x, y);};
	DWORD &operator()(Point <int> p) {return colorArr(p); };

	void Update() {
		  for(int x=_visibleCol; x<_visibleColCount+_visibleCol; x++)
			  for(int y=_visibleRow; y<_visibleRowCount+_visibleRow; y++)
				  quadArr(x, y).setColor(colorArr(x, y));
	};
	void Render() {
		for(int x=_visibleCol; x<_visibleColCount+_visibleCol; x++)
			for(int y=_visibleRow; y<_visibleRowCount+_visibleRow; y++)
				hge->Gfx_RenderQuad(&quadArr(x, y).getOffsetQuad(-_pos.x, -_pos.y));
	};
	void setBackground(DWORD color) {
		_background = color;
	};
	void Clear() {
		for(int x=0; x<_colCount; x++)
			for(int y=0; y<_rowCount; y++)
				colorArr(x, y) = _background;
	};
	bool getMousePos(int mpx, int mpy, int *colPos, int *rowPos) {
		if (mpx<_screenWidth && mpy<_screenHeight &&
			mpx+_pos.x >= 0 && mpy+_pos.y >= 0 &&
			mpx+_pos.x < _totalW && mpy+_pos.y < _totalH) 
		{
			*colPos = ((mpx+_pos.x+_border/4)/(_cellWidth+_border));
			*rowPos = ((mpy+_pos.y+_border/4)/(_cellHeight+_border));
			return true;
		} else {
			*colPos = -1;
			*rowPos = -1;
			return false;
		}
	};
	bool checkVisiblity(int x, int y) {
		if (x>=_visibleCol && y>=_visibleRow &&
			x<_visibleColCount + _visibleCol &&
			y<_visibleRowCount + _visibleRow) 
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
				_poly.v[i].x = (*p)[i].x*(_cellWidth+_border+_border/2)+_cellWidth/2;
				_poly.v[i].y = (*p)[i].y*(_cellHeight+_border+_border/2)+_cellHeight/2;
			}
			hge->Gfx_RenderTriple(&_poly);
			p++;
		}	
	};

	int getColCount() {	return _colCount;};
	int getRowCount() { return _rowCount;};

	int getCellWidth() {return _cellWidth;};
	int getCellHeight() {return _cellHeight;};

	int getVisibleX() {return _pos.x;};
	int getVisibleY() {return _pos.y;};
	
	void setZoom(float coef) {
		Vector <int> deltaPos(_screenWidth/2, _screenHeight/2);
		Vector <int> tempPos = _pos;
		Vector <int> tempCenterPos = _pos+deltaPos;
		Vector <int> newPos;

		int tempTotalW = _totalW;
		int tempTotalH = _totalH;

		if (coef >= 1) {
		_border= (int) (coef/2);

		float maxResoluution = getMin(_screenWidth, _screenHeight);
		
		_cellWidth = (maxResoluution*coef)/(float) _colCount - (float) _border;
		_cellHeight = (maxResoluution*coef)/(float) _rowCount - (float) _border;

		_totalW = _colCount*(_cellWidth+_border);
		_totalH = _rowCount*(_cellHeight+_border);

			for(int _x=0; _x<_colCount; _x++)
				for(int _y=0; _y<_rowCount; _y++)
					quadArr(_x, _y).setQuad(_cellWidth*_x+_border*_x+_border/2,
					_cellHeight*_y+_border*_y +_border/2, 
					_cellWidth, _cellHeight);

			newPos.x = (float)tempCenterPos.x/(float)tempTotalW * (float)_totalW - deltaPos.x;
			newPos.y = (float)tempCenterPos.y/(float)tempTotalH * (float)_totalH - deltaPos.y;
			setPos(newPos);
		}
	}

	Vector <int> getPos() { return _pos; }
	
	void setCenterPos(Vector <int> pos) {
		setCenterPos(pos.x, pos.y);
	}
	void setCenterPos(int x, int y) {
		setPos(_totalW/2 - x, _totalH/2 - y);
	}
	void setPos(Vector <int> pos) {
		setPos(pos.x, pos.y);
	}
	void setPos(int x, int y) {
		_pos.x = x;
		_pos.y = y;
		_visibleCol = x/(_cellWidth+_border)-1;
		_visibleRow = y/(_cellHeight+_border)-1; 
		if (_visibleCol < 0) _visibleCol = 0;
		if (_visibleRow < 0) _visibleRow = 0;
		_visibleColCount = (_screenWidth/(_cellWidth+_border))+3;
		_visibleRowCount = (_screenHeight/(_cellHeight+_border))+3;
		if (_visibleColCount + _visibleCol >= _colCount) _visibleColCount = _colCount-_visibleCol;
		if (_visibleRowCount + _visibleRow >= _rowCount) _visibleRowCount = _rowCount-_visibleRow;
	}
};