#include "Gui_window.h"

GUI_window::GUI_window(HGE *_hge, float _x, float _y, float _w, float _h, DWORD _colorBar, DWORD _colorBack, HTEXTURE *_texCloseBut):
	x(_x),
	y(_y),
	w(_w),
	h(_h),
	hge(_hge),
	visible(true)
{
	gui = new hgeGUI();

	closeBut = new hgeGUIButton(0, x+w-20, y, 20, 20, *_texCloseBut, 0, 0);
	closeBut->SetMode(false);
	gui->AddCtrl(closeBut);
	
	gui->SetColor(0xFFFF0000);
	
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
	bar.v[0].x=x;
	bar.v[0].y=y;
	bar.v[1].x=x+w;
	bar.v[1].y=y;
	bar.v[2].x=x+w;
	bar.v[2].y=y+20;
	bar.v[3].x=x;
	bar.v[3].y=y+20;
	bar.blend=BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE;
	bar.tex=0;
	for(int i=0;i<4;i++) {
		bar.v[i].z=0.5f;
		bar.v[i].col=_colorBar;
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
		hge->Gfx_RenderQuad(&bar);
		gui->Render();
	}
}

void GUI_window::moveWindow(int _x, int _y) {
	setQuadPos(&background, _x, _y);
	setQuadPos(&bar, _x, _y);
	gui->MoveCtrl(0, _x+w-20, _y);
}

void GUI_window::Update(float dt) {
	if (visible) gui->Update(dt);
	if (closeBut->GetState()) visible = false;
}