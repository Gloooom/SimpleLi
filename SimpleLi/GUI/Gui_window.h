#include "Gui_inclusion.h"

#define GetWinButton(win, ctrlname) ((hgeGUIButton*)(win->getCtrl(ctrlname)))
#define GetWinSlider(win, ctrlname) ((hgeGUISlider*)(win->getCtrl(ctrlname)))
#define GetWinListbox(win, ctrlname) ((hgeGUIListbox*)(win->getCtrl(ctrlname)))

#define GetWinButtonState(win, ctrlname) (((hgeGUIButton*)(win->getCtrl(ctrlname)))->GetState())
#define GetWinSliderValue(win, ctrlname) (((hgeGUISlider*)(win->getCtrl(ctrlname)))->GetValue())
#define GetWinListboxSelect(win, ctrlname) (((hgeGUIListbox*)(win->getCtrl(ctrlname)))->GetSelectedItem())

#define SetWinButtonState(win, ctrlname, state) (((hgeGUIButton*)(win->getCtrl(ctrlname)))->SetState(state))
#define SetWinSliderValue(win, ctrlname, val) (((hgeGUISlider*)(win->getCtrl(ctrlname)))->SetValue(val))
#define SetWinListboxSelect(win, ctrlname, select) (((hgeGUIListbox*)(win->getCtrl(ctrlname)))->SetSelectedItem(select))
#define SetWinText(win, ctrlname, text) (((hgeGUIText*)(win->getCtrl(ctrlname)))->SetText(text))

#define SetWinLink(win, sliderID, textID) (SetWinText(win, textID, Convert(GetWinSliderValue(win, sliderID)).c_str()))
#define SetWinValLink(win, value, textID) (SetWinText(win, textID, Convert(value).c_str()))


extern HGE *hge;

struct objInfo {
	int		ID;
	float	x, y;
	void	(*func)();
	bool	doneFlag;
	int		state;
	int		selectItem;
	std::string type;
};

void nullf();

class GUI_window {
public:
	int				ID;
	float			x, y;
	float			w, h;
public:
	bool			visible;
public:
	std::vector	<hgeQuad> graphic;
private:
	std::string		title;
	hgeGUIText		*titleText;
	hgeGUI			*gui;
	float			m_dx, m_dy;
	bool			touchFlag;
	bool			buttonDoneFlag;
	bool			buttonPressFlag;
	hgeQuad			background;
	hgeGUIButton	*titleBar;
	hgeGUIButton	*closeBut;
	HTEXTURE		texBar;
	HTEXTURE		closeButTex;
	static int		objCount;
	std::vector <hgeGUIObject*> objects;
	std::map <std::string, objInfo> objectsID;

private:
	void setQuadPos(hgeQuad *quad, float _x, float _y);

protected:
	GUI_window( 
		float _w, float _h, 
		std::string _title, hgeFont *fnt,
		DWORD _colorBar, DWORD _colorBack, DWORD _colorCloseBut);

	~GUI_window() {
	};
	
	virtual void UpdateFunc() = 0;

public:
	void setColor(DWORD color);
	void setAColor(BYTE alpha);
	void Update(float dt, float mx, float my);
	void Render();
	void setPos(int _x, int _y);
	void addCtrl(hgeGUIObject* obj, float _x, float _y, std::string name, void (*func)() = nullf);
	hgeGUIObject *getCtrl(std::string name);
	objInfo getCtrlInfo(std::string name);	
};



/*
//Пример создания потомка класса окна.

class Win_example : public GUI_window {
private:
	static Win_example *self;
private:
	Win_example(DWORD _headCol, DWORD _backCol, DWORD _objsColor):
	   GUI_window(0, 0, "", fnt, _headCol, _backCol, _objsColor) {
	   //тут описываются элементов интерфейса
	   }
private:
	void UpdateFunc() {
		//обработка элементов интерфейса
	};

public:
	static Win_example *Create(DWORD _headCol, DWORD _backCol, DWORD _objsColor) {
		if (self == NULL) {
			self = new Win_example(_headCol, _backCol, _objsColor);
		}
		return self;
	}

	static void Destroy() {
		delete self;
	};
};
Win_example *Win_example::self = NULL;

*/