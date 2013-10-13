#include "Gui_window.h"
#include "Gui_color.h"

#define OBJ_CLOSE_BUT 0
#define OBJ_TITLE_BAR 1
#define OBJ_TITLE_TEX 2

int GUI_window::objCount = 3;
void nullf() {};

GUI_window::GUI_window(
	float _w, float _h, 
	std::string _title, hgeFont *fnt, 
	DWORD _colorBar, DWORD _colorBack, DWORD _colorCloseBut):
	x(0),
	y(0),
	w(_w),
	h(_h),
	visible(true),
	buttonDoneFlag(false),
	buttonPressFlag(false),
	title(_title)
{
	gui = new hgeGUI();

	closeButTex = getButtonTex(15, 15, _colorCloseBut, 0.15);

	texBar = hge->Texture_Create(1, 1);
	DWORD *b_ptr = hge->Texture_Lock(texBar, false, 0, 0, 1, 1);
	b_ptr[0] = _colorBar;
	hge->Texture_Unlock(texBar);
	
	closeBut = new hgeGUIButton(OBJ_CLOSE_BUT, x+w-15, y, 15, 15, closeButTex, 0, 0);
	closeBut->SetMode(false);
	
	titleBar = new hgeGUIButton(OBJ_TITLE_BAR, x, y, w, 15, texBar, 0, 0);
	
	titleText = new hgeGUIText(OBJ_TITLE_TEX, x+5, y+5, w, 15, fnt);	
	titleText->SetMode(HGETEXT_LEFT);
	titleText->SetColor(0xFFFFFFFF);
	titleText->SetText(title.c_str());
	titleText->bEnabled = false;
	
	gui->AddCtrl(titleBar);
	gui->AddCtrl(titleText);
	gui->AddCtrl(closeBut);

	background.v[0].x=x;
	background.v[0].y=y;
	background.v[1].x=x+w;
	background.v[1].y=y;
	background.v[2].x=x+w;
	background.v[2].y=y+h;
	background.v[3].x=x;
	background.v[3].y=y+h;
	background.blend=BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE;
	background.tex=0;
	for(int i=0;i<4;i++) {
		background.v[i].z=0.5f;
		background.v[i].col=_colorBack;
	}
};

void GUI_window::setQuadPos(hgeQuad *quad, float _x, float _y) {
	quad->v[1].x-=quad->v[0].x;
	quad->v[2].x-=quad->v[0].x;
	quad->v[2].y-=quad->v[0].y;
	quad->v[3].y-=quad->v[0].y;
	quad->v[0].x=_x;
	quad->v[0].y=_y;
	quad->v[1].y=_y;
	quad->v[3].x=_x;
	quad->v[1].x+=_x;
	quad->v[2].x+=_x;
	quad->v[2].y+=_y;
	quad->v[3].y+=_y;
};

void GUI_window::Render() {
	if (visible) {
		hge->Gfx_RenderQuad(&background);
		std::vector <hgeQuad> ::iterator q = graphic.begin();
		while(q != graphic.end()) {
			hge->Gfx_RenderQuad(&*q);
			q++;
		}
		gui->Render();
	}
}

void GUI_window::setPos(int _x, int _y) { //баг: перемещается кнопка закрытия окна в левый верхний угол
	setQuadPos(&background, _x, _y);
	gui->MoveCtrl(OBJ_CLOSE_BUT, _x+w-15, _y);
	gui->MoveCtrl(OBJ_TITLE_BAR, _x, _y);
	gui->MoveCtrl(OBJ_TITLE_TEX, _x+5, _y+5);
	std::map <std::string, objInfo> ::iterator p = objectsID.begin();
	while (p != objectsID.end()) {
		gui->MoveCtrl(p->second.ID, _x+p->second.x, _y+p->second.y);
		p++;
	}
	std::vector <hgeQuad> ::iterator q = graphic.begin();
	while(q != graphic.end()) {
		setQuadPos(&*q, q->v[0].x - x +_x, q->v[0].y - y + _y);
		q++;
	}
	x = _x;
	y = _y;
}

void GUI_window::Update(float dt, float mx, float my) {
	gui->Update(dt);
	std::map <std::string, objInfo> ::iterator p = objectsID.begin();
	while (p != objectsID.end()) {
		if (p->second.type == "class hgeGUIButton") {
			if (!p->second.state) {
				if (hgeButtonGetState(gui,p->second.ID)) {
					if (p->second.doneFlag && !buttonPressFlag) {
						p->second.doneFlag = false;
						buttonPressFlag = true;
					}
				} else if (!p->second.doneFlag) {
					p->second.func();
					p->second.doneFlag = true;
					buttonPressFlag = false;
				}
			} else {
				if (hgeButtonGetState(gui,p->second.ID))
					p->second.func();
			}
		} else if (p->second.type == "class hgeGUIListbox") {
			p->second.selectItem = ((hgeGUIListbox*)gui->GetCtrl(p->second.ID))->GetSelectedItem();
			if(p->second.state != p->second.selectItem) {
				p->second.state = p->second.selectItem;
				p->second.func();
			}
		}
		p++;
	}
	UpdateFunc();
	if (closeBut->GetState()) {
		visible = false;
		buttonPressFlag = false;
		closeBut->SetState(false);
	}
	if (titleBar->GetState()) {
		if (!touchFlag) {
			touchFlag = true;
			m_dx = mx - x;
			m_dy = my - y;
		}
		setPos(mx - m_dx, my - m_dy);
	} else {touchFlag = false;}
}

//При добавлении кнопки, и передачи в метод ссылки на функцию, она будет выполнена по нажатию на кнопку.
//При добавлении листбокса, и передачи в метод ссылки на функцию, она будет выполнена после смены выбранного элемента.
void GUI_window::addCtrl(hgeGUIObject* obj, float _x, float _y, std::string name, void (*func)()) {
	if (objectsID.find(name) == objectsID.end()) {
		objectsID[name].ID = objCount;
		objectsID[name].x = _x;
		objectsID[name].y = _y;
		objectsID[name].func = func;
		objectsID[name].doneFlag = true;
		objectsID[name].selectItem = 0;
		objectsID[name].state = 0;
		objectsID[name].type = typeid(*obj).name();
	}

	if (typeid(*obj).name() == typeid(hgeGUISlider).name()) {
		hgeGUISlider *slid;
		slid = new hgeGUISlider(*((hgeGUISlider*)obj));
		slid->sprSlider = new hgeSprite (*((hgeGUISlider*)obj)->sprSlider);
		objects.push_back(slid);
	} else if (typeid(*obj).name() == typeid(hgeGUIListbox).name()) {
		hgeGUIListbox *listbox;
		listbox = new hgeGUIListbox(*((hgeGUIListbox*)obj));
		listbox->sprHighlight = new hgeSprite (*((hgeGUIListbox*)obj)->sprHighlight);
		objects.push_back(listbox);
	} else if (typeid(*obj).name() == typeid(hgeGUIButton).name()) {
		hgeGUIButton *but;
		but = new hgeGUIButton(*((hgeGUIButton*)obj));
		but->sprUp = new hgeSprite (*((hgeGUIButton*)obj)->sprUp);
		but->sprDown = new hgeSprite (*((hgeGUIButton*)obj)->sprDown);
		objectsID[name].state = but->GetMode();
		objects.push_back(but);
	} else if (typeid(*obj).name() == typeid(hgeGUIText).name())
		objects.push_back(new hgeGUIText(*((hgeGUIText*)obj)));
	
	
	(*(--objects.end()))->id = objCount;
	(*(--objects.end()))->SetPos(_x, _y);

	gui->AddCtrl(*(--objects.end()));

	objCount++;
}

hgeGUIObject *GUI_window::getCtrl(std::string name) {
	return gui->GetCtrl(objectsID[name].ID);
}

objInfo GUI_window::getCtrlInfo(std::string name) {
	return objectsID[name];
}

void GUI_window::setColor(DWORD color) {
	for(int i=0;i<4;i++) {
		background.v[i].col = color;
	}
}

void GUI_window::setAColor(BYTE alpha) {
	union {
		DWORD dw;
		unsigned char b[4];
	} _col;
	for(int i=0;i<4;i++) {
		_col.dw = background.v[i].col;
		_col.b[3] = alpha;
		background.v[i].col = _col.dw;
	}

	std::vector <hgeQuad> ::iterator q = graphic.begin();
	while(q != graphic.end()) {
		for(int i=0;i<4;i++) {
			_col.dw = q->v[i].col;
			_col.b[3] = alpha;

			q->v[i].col = _col.dw;
		}
		q++;
	}
}