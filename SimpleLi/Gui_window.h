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


struct objInfo {
	int ID;
	float x, y;
};

class Pixel {
private:
	HTEXTURE pix;
	HGE *hge;
public:
	Pixel(DWORD color, HGE *_hge) {
		hge = _hge;
		pix = hge->Texture_Create(1, 1);
		setCol(color);
	};
	Pixel(HGE *_hge) {
		hge = _hge;
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
	HGE *hge;
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
	GUI_window(HGE *_hge, 
		float _w, float _h, 
		std::string _title, hgeFont *fnt,
		DWORD _colorBar, DWORD _colorBack, HTEXTURE *_texCloseBut);

	~GUI_window() {
		//delete gui;
		//delete closeBut;
	};

	void Update(float dt, float mx, float my);
	void Render();
	void setPos(int _x, int _y);
	void addCtrl(hgeGUIObject* obj, float _x, float _y, std::string name);
	hgeGUIObject *getCtrl(std::string name);
	//Заготовка для окна с интерфейсом
	//обязательно должна присутствовать возможность перетаскивания
	//с использованием этого класса реализовать окошко отображения одной особи с её областями видимости.
};

class GUI_manager {
private:
	std::map <int, GUI_window> windows;
	std::deque <int> activeWindows;
	hgeGUI *gui;
	Pixel diaPix;
public:
	GUI_manager(HGE *hge):
	diaPix(0x00000000, hge)
	{
		gui = new hgeGUI();
	};
	void addWindow(GUI_window win, int ID) {
		win.ID = ID;
		windows[win.ID] = win;

		gui->AddCtrl(new hgeGUIButton(win.ID, win.x, win.y, win.x+win.w, win.y+win.h, diaPix.getPix(), 0, 0));
		((hgeGUIButton*)gui->GetCtrl(win.ID))->SetMode(false);
	};
	GUI_window *getFocusWin() {
		return &windows.at(activeWindows.front());
	};
	int getFocusID() {
		return activeWindows.front();
	};
	void Update(float dt, float mx, float my) {
		gui->Update(dt);
		std::deque <int> ::reverse_iterator but = activeWindows.rbegin();
		while (but != activeWindows.rend()) {
			if(hgeButtonGetState(gui, *but)) setActive(*but);
			but++;
		}
		if (!activeWindows.empty()) {
			gui->MoveCtrl(getFocusID(), windows[getFocusID()].x, windows[getFocusID()].y);
			getFocusWin()->Update(dt, mx, my);
			if (!getFocusWin()->visible) activeWindows.erase(activeWindows.begin());
		}
	};
	void Render() {
		std::deque <int> ::reverse_iterator p = activeWindows.rbegin();
		while (p != activeWindows.rend()) {
			windows.at(*p).Render();
			p++;
		}

	};
	void setActive(int ID) {
		std::deque <int> ::iterator futureFrontID = std::find(activeWindows.begin(), activeWindows.end(), ID);
		if (futureFrontID == activeWindows.end()) {
			activeWindows.push_front(ID);
		} else {
			activeWindows.erase(futureFrontID);
			activeWindows.push_front(ID);
		}
	}
};
