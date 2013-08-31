#include "Gui_inclusion.h"

extern HGE *hge;

struct objInfo {
	int		ID;
	float	x, y;
	void	(*func)();
	bool	doneFlag;
	bool	check;
};

void nullf();

class GUI_window {
public:
	int				ID;
	float			x, y;
	float			w, h;
public:
	bool			visible;
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

public:
	GUI_window() {};
	GUI_window( 
		float _w, float _h, 
		std::string _title, hgeFont *fnt,
		DWORD _colorBar, DWORD _colorBack, DWORD _colorCloseBut);

	~GUI_window() {
	};

	void setColor(DWORD color);
	void setAColor(BYTE alpha);
	void Update(float dt, float mx, float my);
	void Render();
	void setPos(int _x, int _y);
	void addCtrl(hgeGUIObject* obj, float _x, float _y, std::string name, void (*func)() = nullf);
	hgeGUIObject *getCtrl(std::string name);
};

