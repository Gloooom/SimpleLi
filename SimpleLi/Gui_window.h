#include "stdafx.h"

#include "..\HGE\hge.h"
#include "..\HGE\hgefont.h"
#include "..\HGE\hgegui.h"
#include "..\HGE\hgerect.h"
#include "..\HGE\hgecolor.h"
#include "..\HGE\hgegui.h"
#include "..\HGE\hgeguictrls.h"

#define CTRL_SLIDER		0 
#define	CTRL_BUTTON		1
#define	CTRL_TEXT		2
#define CTRL_LISTBOX	3
extern HGE *hge;

struct objInfo {
	int ID;
	float x, y;
};

class GUI_window {
public:
	int ID;
	float x, y; //левый верхний угол
	float w, h;
public:
	bool visible;
private:
	std::string title;
	hgeGUIText *titleText;
	hgeGUI *gui;
	float m_dx, m_dy;
	bool touchFlag;
	hgeQuad background;
	hgeGUIButton *titleBar;
	hgeGUIButton *closeBut;
	HTEXTURE texBar;
	std::vector <hgeGUIObject*> objects;
	std::map <std::string, objInfo> objectsID;
	static int objCount;
private:
	void setQuadPos(hgeQuad *quad, float _x, float _y);

public:
	GUI_window() {};
	GUI_window( 
		float _w, float _h, 
		std::string _title, hgeFont *fnt,
		DWORD _colorBar, DWORD _colorBack, HTEXTURE *_texCloseBut);

	~GUI_window() {
	};

	void setAColor(BYTE alpha);
	void Update(float dt, float mx, float my);
	void Render();
	void setPos(int _x, int _y);
	void addCtrl(hgeGUIObject* obj, float _x, float _y, std::string name);
	hgeGUIObject *getCtrl(std::string name);
};

