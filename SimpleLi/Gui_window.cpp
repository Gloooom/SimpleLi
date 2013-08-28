#include "Gui_window.h"

int GUI_window::objCount = 3;

GUI_window::GUI_window(HGE *_hge, 
	float _w, float _h, 
	std::string _title, hgeFont *fnt, 
	DWORD _colorBar, DWORD _colorBack, HTEXTURE *_texCloseBut):
	x(0),
	y(0),
	w(_w),
	h(_h),
	hge(_hge),
	visible(true),
	title(_title)
{
	gui = new hgeGUI();

	texBar = hge->Texture_Create(1, 1);
	DWORD *b_ptr = hge->Texture_Lock(texBar, false, 0, 0, 1, 1);
	b_ptr[0] = _colorBar;
	hge->Texture_Unlock(texBar);
	
	closeBut = new hgeGUIButton(0, x+w-20, y, 20, 20, *_texCloseBut, 0, 0);
	closeBut->SetMode(false);
	
	titleBar = new hgeGUIButton(1, x, y, w, 20, texBar, 0, 0);
	closeBut->SetMode(false);
	
	titleText = new hgeGUIText(2, x+5, y+5, w, 20, fnt);	
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
		gui->Render();
	}
}

void GUI_window::setPos(int _x, int _y) {
	x = _x;
	y = _y;
	setQuadPos(&background, _x, _y);
	gui->MoveCtrl(0, _x+w-20, _y);
	gui->MoveCtrl(1, _x, _y);
	gui->MoveCtrl(2, _x+5, _y+5);
	std::map <std::string, objInfo> ::iterator p = objectsID.begin();
	while (p != objectsID.end()) {
		gui->MoveCtrl(p->second.ID, _x+p->second.x, _y+p->second.y);
		p++;
	}
}

void GUI_window::Update(float dt, float mx, float my) {
	if (visible) {
		gui->Update(dt);
		if (closeBut->GetState()) visible = false;
		if (titleBar->GetState()) {
			if (!touchFlag) {
				touchFlag = true;
				m_dx = mx - x;
				m_dy = my - y;
			}
			setPos(mx - m_dx, my - m_dy);
		} else {touchFlag = false;}
	}
}

void GUI_window::addCtrl(hgeGUIObject* obj, float _x, float _y, std::string name) {
	if (objectsID.find(name) == objectsID.end()) {
		objectsID[name].ID = objCount;
		objectsID[name].x = _x;
		objectsID[name].y = _y;
	}
	obj->id = objCount;
	objects.push_back(obj);
	gui->AddCtrl(obj);
	objCount++;
}

hgeGUIObject *GUI_window::getCtrl(std::string name) {
	return gui->GetCtrl(objectsID[name].ID);
}

