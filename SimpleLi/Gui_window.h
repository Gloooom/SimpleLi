#include "stdafx.h"

#include "..\HGE\hge.h"
#include "..\HGE\hgefont.h"
#include "..\HGE\hgegui.h"
#include "..\HGE\hgerect.h"
#include "..\HGE\hgecolor.h"
#include "..\HGE\hgegui.h"
#include "..\HGE\hgeguictrls.h"

class GUI_window {
private:
	HGE *hge;
	hgeGUI *gui;
	float x, y; //верхний левый угол
	float w, h;
	bool visible;
	std::string title;
	hgeQuad background;
	hgeQuad bar;
	hgeGUIButton *closeBut;
	std::vector <hgeGUIObject*> objects;
	std::map <int, std::string> objectsID;
private:
	void setQuadPos(hgeQuad *quad, float _x, float _y);
public:
	GUI_window(HGE *_hge, float _x, float _y, float _w, float _h, DWORD _colorBar, DWORD _colorBack, HTEXTURE *_texCloseBut);

	~GUI_window() {
		delete gui;
	};

	void Update(float dt);
	void Render();
	void moveWindow(int _x, int _y);
	void addCtrl(hgeGUIObject*, std::string name) {};
	void getValue(std::string name) {};
	//Заготовка для окна с интерфейсом
	//обязательно должна присутствовать возможность перетаскивания
	//с использованием этого класса реализовать окошко отображения одной особи с её областями видимости.
};

class GUI_manager {
private:
	std::vector <GUI_window> windows;
public:
	void addWindow(GUI_window win) {

	}
	//класс для хранения всех окон и контроля активного окна
};
