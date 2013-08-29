

#include "..\HGE\hge.h"
#include "..\HGE\hgefont.h"
#include "..\HGE\hgegui.h"
#include "..\HGE\hgerect.h"
#include "..\HGE\hgecolor.h"
#include "..\HGE\hgeparticle.h"
#include "..\HGE\hgevector.h"
#include "..\HGE\hgeanim.h"
#include "..\HGE\hgegui.h"
#include "..\HGE\hgeguictrls.h"

#define ButtonGetState(id)		((hgeGUIButton*)gui->GetCtrl(id))->GetState()
#define ButtonSetState(id,b)	((hgeGUIButton*)gui->GetCtrl(id))->SetState(b)
#define SliderGetValue(id)		((hgeGUISlider*)gui->GetCtrl(id))->GetValue()
#define SliderSetValue(id,f)	((hgeGUISlider*)gui->GetCtrl(id))->SetValue(f)
#define GetTextCtrl(id)			((hgeGUIText*)gui->GetCtrl(id))

#pragma once

extern HGE *hge;

double Dot(hgeVector hgev1, hgeVector hgev2);

struct PEditorState {
	Point <float> mp;
};

struct Cell {
	double x, y;
	DWORD color;
	double width, height;
};

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

	HTEXTURE getPix() {return pix;};
	void	 setCol(DWORD color) {
		DWORD *ptr = hge->Texture_Lock(pix, false, 0, 0, 1, 1);
		ptr[0] = color;
		hge->Texture_Unlock(pix);
	};
};

class Line {
private:
	float width;

	float x1, y1, x2, y2;
	DWORD color;
	hgeQuad q;
public:
	bool isRender;

	Line() {
		Line t(0, 0, 1, 1, 2);
		*this=t;
	}

	hgeQuad* getQuad() {
		return &q;
	}

	void RenderLine() {
		hge->Gfx_RenderQuad(&q);
	}

	Line(float _x1, float _y1, float _x2, float _y2, float _width, DWORD _color=0xFFFFFFFF) {
		isRender=true;
		width=_width;
		x1=_x1;
		y1=_y1;
		x2=_x2;
		y2=_y2;
		color=_color;

		hgeVector t(1, 0), pos1(x1, y1), pos2(x2, y2);
		hgeVector R=pos2-pos1;

		float angle;
		if (pos2.y>pos1.y)
			angle=2*M_PI-Dot(R, t);
		else
			angle=Dot(R, t);

		float coefS=sin(angle);
		float coefC=cos(angle);

		q.v[0].x=pos1.x-(width/2)*coefS;
		q.v[0].y=pos1.y-(width/2)*coefC;

		q.v[1].x=pos1.x+(width/2)*coefS;
		q.v[1].y=pos1.y+(width/2)*coefC;

		q.v[2].x=pos2.x+(width/2)*coefS;
		q.v[2].y=pos2.y+(width/2)*coefC;

		q.v[3].x=pos2.x-(width/2)*coefS;
		q.v[3].y=pos2.y-(width/2)*coefC;

		q.blend=BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE;
		q.tex=0;
		for(int i=0;i<4;i++) {
			q.v[i].z=0.5f;
			q.v[i].col=color;
		}
	}

	void setPos(float _x1, float _y1, float _x2, float _y2) {
		bool isRenderTemp=isRender;
		Line t(_x1, _y1, _x2, _y2, width, color);
		t.isRender=isRenderTemp;
		*this=t;
	}

	void setColor(DWORD _color) {
		color=_color;

		for(int i=0;i<4;i++) {
			q.v[i].z=0.5f;
			q.v[i].col=color;
		}	
	}

	void setWidth(float _width) {
		bool isRenderTemp=isRender;
		Line t(x1, y1, x2, y2, _width, color);
		t.isRender=isRenderTemp;
		*this=t;
	}
};


extern hgeFont		*fnt;
extern hgeGUI		*gui;
extern hgeSprite	*sprCell;
extern HTEXTURE		texGui, texBut, texCell;

extern PEditorState state;
extern GeneticCode g;


#define CMD_EXIT				1
#define CMD_HELP				2
#define GUI_SLIDER				3
#define GUI_TEXT				4
