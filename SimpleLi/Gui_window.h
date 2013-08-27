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
	std::string title;
	hgeGUIText *titleText;
	int ID;
	HGE *hge;
	hgeGUI *gui;
	float x, y; //левый верхний угол
	float w, h;
	bool visible;
	float m_dx, m_dy;
	bool touchFlag;
	hgeQuad background;
	hgeGUIButton *titleBar;
	hgeGUIButton *closeBut;
	HTEXTURE texBar;
	std::vector <hgeGUIObject*> objects;
	std::map <std::string, int> objectsID;
private:
	void setQuadPos(hgeQuad *quad, float _x, float _y);
public:
	GUI_window(HGE *_hge, 
		float _x, float _y, float _w, float _h, 
		std::string _title, hgeFont *fnt,
		DWORD _colorBar, DWORD _colorBack, HTEXTURE *_texCloseBut);

	~GUI_window() {
		delete gui;
		delete closeBut;
	};

	void Update(float dt, float mx, float my);
	void Render();
	void setPos(int _x, int _y);
	void addCtrl(hgeGUIObject* obj, std::string name) {};
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
	//обработку нажатий осуществлять с помощью массива окон, где объекты расположенны
	//по их физическому расположению в интерфейсе (нижнее в начале вектора, активное в самом конце)
	//обработку же осуществлять по анализу положения мыши или как-то иначе(КАК???)
	//координаты левого верхнего и правого нижнего углов каждого окна как-то вывести в доступ
};
