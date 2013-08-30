
#include "stdafx.h"


#pragma comment (lib, "hge.lib")
#pragma comment (lib, "hgehelp.lib")

#pragma once

#include "Logic\Individ.h"
#include "Logic\Environment.h"
#include "Logic\Types.h"
#include "Gui_win_manager.h"
#include "Gui_types.h"
#include "Gui_color.h"

HGE *hge=0;


hgeFont			*fnt;
hgeGUI			*gui;

HTEXTURE		texGui, texBut, texCell;

hgeSprite		*sprLeftPane1, *sprLeftPane2, *sprRightPane1, *sprRightPane2;
hgeSprite		*testSpr;
		
PEditorState	state;

GUI_win_manager *winManager;

Cell Field[W][H];
double border=0;

Environment env;
Individ *selectInd=&(env.empty);

float			psx=400, psy=300;
float			timer=0;
bool			play=true;

int wincount = 3;
void			InitEditor();
void			DoneEditor();
void			CreateGUI();
void			InitEnvironment();
void			addIndivid(Point <float> p, Mode_feeding diet); 


void nullF() {};



void fff() {
	env.save("fff.bin");
		//GUI_window *test_w_new = new GUI_window(
		//	((hgeGUISlider*)winManager->getWin(1)->getCtrl("w"))->GetValue(), 
		//	((hgeGUISlider*)winManager->getWin(1)->getCtrl("h"))->GetValue(), 
		//	"Title_new", fnt, 0xFFAAAAAA, 0xFF999999, &texCell);
		//winManager->addWindow(test_w_new, wincount);
		//winManager->setActive(wincount);
		//wincount++;
};

bool FrameFunc()
{
	float		px, py;
	float		dt=hge->Timer_GetDelta();
	
	hge->Input_GetMousePos(&state.mp.x, &state.mp.y);

	winManager->Update(dt, state.mp.x, state.mp.y);

	gui->Update(dt);
	
	HSVColor color(0xFFFF0000);
	color.setHue((((hgeGUISlider*)winManager->getWin(1)->getCtrl("h"))->GetValue()/100.00f)*360.00f);
	color.setSaturation(((hgeGUISlider*)winManager->getWin(1)->getCtrl("s"))->GetValue()/100.00f);
	color.setValue(((hgeGUISlider*)winManager->getWin(1)->getCtrl("v"))->GetValue()/100.00f);
	winManager->getWin(1)->setColor(color);

	if (play) {
		timer+=dt;
		if (timer>0.1) {

			env.step();
			
			for (int _x=0; _x<W; _x++) 
				for (int _y=0; _y<H; _y++) 
					if (*env.field[_x][_y] != env.empty) {
						if (env.field[_x][_y]->gender == MALE)
							Field[_x][_y].color=0xFF0000FF;
						if (env.field[_x][_y]->gender == FEMALE)
							Field[_x][_y].color=0xFFFF0000;
					} else {
						Field[_x][_y].color=0xFFFFFFFF;
					}

			timer=0;
		}
	}

	return false;
}


bool RenderFunc()
{
	hge->Gfx_Clear(0);
	hge->Gfx_BeginScene();
	
	hgeQuad cellQ;
	for (int x=0; x<W; x++) {
		for(int y=0; y<H; y++) {
			cellQ.v[0].x=Field[x][y].x;
			cellQ.v[0].y=Field[x][y].y;
			cellQ.v[0].col=Field[x][y].color;
			cellQ.v[0].z=0.5f;

			cellQ.v[1].x=Field[x][y].x;
			cellQ.v[1].y=Field[x][y].y+Field[x][y].height;
			cellQ.v[1].col=Field[x][y].color;
			cellQ.v[1].z=0.5f;

			cellQ.v[2].x=Field[x][y].x+Field[x][y].width;
			cellQ.v[2].y=Field[x][y].y+Field[x][y].height;
			cellQ.v[2].col=Field[x][y].color;
			cellQ.v[2].z=0.5f;

			cellQ.v[3].x=Field[x][y].x+Field[x][y].width;
			cellQ.v[3].y=Field[x][y].y;
			cellQ.v[3].col=Field[x][y].color;
			cellQ.v[3].z=0.5f;

			cellQ.blend=BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE;
			cellQ.tex=0;
			
			hge->Gfx_RenderQuad(&cellQ);
		}
	}

	std::map <long long int, Individ> ::iterator p = env.population.begin();
	while (p != env.population.end()) {
		Cell c = Field[p->second.pos.x][p->second.pos.y];
		float x1, y1, x2, y2;
		x1 = func::round(c.x+c.width/2);
		y1 = func::round(c.y+c.height/2);
		x2 = x1-p->second.way.x*20;
		y2 = y1-p->second.way.y*20;
		Line l(x1, y1, x2, y2, 5, 0x55009999);
		l.RenderLine();
		p++;
	}

	
	gui->Render();
	winManager->Render();

	std::string statuses[] = {"HUNGRY", "EAT", "MATURE", "REPRODUCT", "WAIT"};
	if (!env.population.empty())
	fnt->printf(605, 5, HGETEXT_LEFT, 
	"FPS: %d "
	"\nPopulation: %d"
	"\nStep: %d",
	hge->Timer_GetFPS(), 
	env.population.size(), 
	(int) env.stepCount
	);
		
	hge->Gfx_EndScene();

	return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	hge = hgeCreate(HGE_VERSION);

	hge->System_SetState(HGE_INIFILE, "SimpleLi.ini");
	hge->System_SetState(HGE_LOGFILE, "SimpleLi.log");
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_FOCUSGAINFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	hge->System_SetState(HGE_TITLE, "SimpleLi");
	hge->System_SetState(HGE_WINDOWED, 1);
	hge->System_SetState(HGE_HIDEMOUSE, 0);
	hge->System_SetState(HGE_FPS, 100);
	hge->System_SetState(HGE_SCREENWIDTH, 800);
	hge->System_SetState(HGE_SCREENHEIGHT, 600);
	hge->System_SetState(HGE_SCREENBPP, 32);
	hge->System_SetState(HGE_USESOUND, false);

	if(hge->System_Initiate()) {
		InitEditor();
		InitEnvironment();
		hge->System_Start();
		DoneEditor();
	} else 
		MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);

	hge->System_Shutdown();
	hge->Release();
	return 0;
}

void addIndivid(Point <float> p, Mode_feeding diet) {
	GeneticCode g;
		g.phis[acceleration] = 0.3;
		g.phis[hp_max] = 10; 
		g.phis[energy_max] = 3000;
		g.phis[saturation] = 10;
		g.phis[stamina] = 2;
		g.phis[fertility] = 4;
		g.phis[live_time] = 400;
		g.phis[reproduction_time] = 10; 
		g.phis[reproduction_pause] = 100;
		g.radialEye.setHeight(3);
		g.eyes.push_back(FOV_Tri(0, 30, 20));
		g.diet=diet;
		for (int i=0; i<end_of_status; i++) {
			if (diet==AUTO) {
				g.soc[i][max_speed] = func::randf(0, 5);
				g.soc[i][rand_way] = func::randf(0, 10);
				g.soc[i][partner] = func::randf(0, 10);
				g.soc[i][cohesion_partner] = func::randf(0, 10);
				g.soc[i][separation_partner] = func::randf(0, 10);
				g.soc[i][alignment_partner] = func::randf(0, 10);
				g.soc[i][enemy] = func::randf(0, 10);
				g.soc[i][cohesion_enemy] = func::randf(0, 10);
				g.soc[i][separation_enemy] = func::randf(0, 10);
				g.soc[i][alignment_enemy] = func::randf(0, 10);
				g.color = 0xFF009900;
			}
			if (diet==GETERO) {
				g.soc[i][max_speed] = 3;
				g.soc[i][rand_way] = 1;
				g.soc[i][partner] = 1;
				g.soc[i][cohesion_partner] = 1;
				g.soc[i][separation_partner] = 0.6f;
				g.soc[i][alignment_partner] = 1.6f;
				g.soc[i][enemy] = 1;
				g.soc[i][cohesion_enemy] = -5;
				g.soc[i][separation_enemy] = 0;
				g.soc[i][alignment_enemy] = 0.0f;
				g.color = 0xFF990000;

			}
		}
		hgeColor c(func::randf(0,1), func::randf(0,1), func::randf(0,1), 1);
		g.color = c.GetHWColor();
		env.addIndivid(Individ(func::round(p), g));
}

void InitEnvironment() {
	Point <float> p;

	for(int i=0; i<POP_A; i++) {
		p.x=func::randf(0, W);
		p.y=func::randf(0, H); 
		addIndivid(p, AUTO);
	}
	for(int i=0; i<POP_G; i++) {
		p.x=func::randf(0, W);
		p.y=func::randf(0, H); 
		addIndivid(p, GETERO);
	}
}

void InitEditor() {
	int bgw, bgh;
	
	texBut=hge->Texture_Load("Button.png");
	texCell=hge->Texture_Load("closeBut.png");

	fnt=new hgeFont("123.fnt");
	fnt->SetScale(0.5);

	Cell typicCell;
	hgeColor color(1, 1, 1, 1);
	typicCell.color=color.GetHWColor();
	typicCell.height=(600-border*(H+1))/H;
	typicCell.width=(600-border*(W+1))/W;
	typicCell.x=0;
	typicCell.y=0;

	for (int x=0; x<W; x++) {
		typicCell.x=(typicCell.width+border)*x+border;
		for(int y=0; y<H; y++) {
			typicCell.y=(typicCell.height+border)*y+border;
			Field[x][y]=typicCell;
		}
	}
	
	HTEXTURE butTex = getButtonTex(40,10,0xFFFFFF22);

	gui=new hgeGUI();
	CreateGUI();

	RGBColor colPix(0xFF00DD00);

	Pixel *test_p = new Pixel(colPix);

	hgeGUISlider *slider;
	slider = new hgeGUISlider(GUI_SLIDER, 0, 0, 180, 10, test_p->getPix(), 0, 0, 1, 1);
	slider->SetMode(0, 100, HGESLIDER_BAR);
	slider->SetValue(100);

	hgeGUIButton *button;
	button = new hgeGUIButton(CMD_EXIT, 0, 0, 40, 10, butTex, 0, 0);
	button->SetMode(false);

	hgeGUIListbox *list;
	list = new hgeGUIListbox(0, 0, 0, 100, 100, fnt, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF);
	list->AddItem("bla-bla1");
	list->AddItem("bla-bla2");
	list->AddItem("bla-bla3");
	list->AddItem("bla-bla4");

	GUI_window *test_w_1;
	test_w_1 = new GUI_window(200, 200, "Make Windows!", fnt, 0xFFAAAAAA, 0xFF999999, &texCell);
	test_w_1->addCtrl(slider, 10, 100, "h", fff);
	test_w_1->addCtrl(slider, 10, 120, "s", fff);
	test_w_1->addCtrl(slider, 10, 140, "v", fff);
	test_w_1->addCtrl(button, 10, 180, "but", fff);
	//test_w_1->addCtrl(list, 10, 50, "list", nullF); 
	winManager = new GUI_win_manager();
	winManager->addWindow(test_w_1, 1);
	winManager->setActive(1);
	winManager->setWinPos(1, 200, 200);
}

void DoneEditor() {
	delete gui;
	delete fnt;

	hge->Texture_Free(texBut);
	hge->Texture_Free(texCell);
}

void CreateGUI() {
	hgeGUIButton *button;
	hgeGUISlider *slider;
	hgeGUIText	 *text;

	HTEXTURE butTex = getButtonTex(40,10,0xFFFFFF22);
	button=new hgeGUIButton(CMD_HELP, 700, 500, 40, 10, butTex, 0, 0);
	button->SetMode(false);
	gui->AddCtrl(button);

	text=new hgeGUIText(GUI_TEXT, 610, 390, 180, 10, fnt);
	text->SetMode(HGETEXT_LEFT);
	text->SetColor(0xFFFFFFFF);
	text->SetText("O");
	gui->AddCtrl(text);
	text->SetText("GG");
	//GetTextCtrl(GUI_TEXT)->printf("OLOLOLO");
}