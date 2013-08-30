
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
hgeGUI			*mainGUI;

HTEXTURE		closeButton;
RGBColor		objsColor;

Pixel			*sliderTexture;
hgeSprite		*testSpr;
		
EditorState	state;

GUI_win_manager *winManager;

Cell Field[W][H];
double border=0;

Environment env;
Individ *selectInd=&(env.empty);

float			psx=400, psy=300;
float			timer=0;
bool			play=true;
int				wincount = 3;
int				testVal = 0;

void			InitEditor();
void			DoneEditor();
void			CreateGUI();
void			CreateWinManager();
void			InitEnvironment();
void			addIndivid(Point <float> p, Mode_feeding diet); 



void save_func() {
	int selectSlot = GetWinListboxSelect(winManager, WIN_S_L, "slot_list");
	switch(selectSlot) {
	case 0: env.save("save0.bin"); break;
	case 1: env.save("save1.bin"); break;
	case 2: env.save("save2.bin"); break;
	case 3: env.save("save3.bin"); break;
	case 4: env.save("save4.bin"); break;
	case 5: env.save("save5.bin"); break;
	}
};

void load_func() {
	int selectSlot = GetWinListboxSelect(winManager, WIN_S_L, "slot_list");
	switch(selectSlot) {
	case 0: env.load("save0.bin"); break;
	case 1: env.load("save1.bin"); break;
	case 2: env.load("save2.bin"); break;
	case 3: env.load("save3.bin"); break;
	case 4: env.load("save4.bin"); break;
	case 5: env.load("save5.bin"); break;
	}
};

bool FrameFunc()
{
	float		px, py;
	float		dt=hge->Timer_GetDelta();
	
	hge->Input_GetMousePos(&state.mp.x, &state.mp.y);

	winManager->Update(dt, state.mp.x, state.mp.y);

	mainGUI->Update(dt);

	if (hgeButtonGetState(mainGUI, CMD_WIN_S_L)) {
		winManager->setActive(WIN_S_L);
		winManager->setFocus(WIN_S_L);
	}
	if (hgeButtonGetState(mainGUI, CMD_WIN_ADD_IND)) {
		winManager->setActive(WIN_ADD_IND);
		winManager->setFocus(WIN_ADD_IND);
	}

	if (state.play) {
		timer+=dt;
		if (timer>0.1) {
			env.step();
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
			if (*env.field[x][y] != env.empty) {
				if (env.field[x][y]->gender == MALE)
					Field[x][y].color=0xFF0000FF;
				if (env.field[x][y]->gender == FEMALE)
					Field[x][y].color=0xFFFF0000;
			} else {
				Field[x][y].color=0xFFFFFFFF;
			}

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
	
	mainGUI->Render();

	std::string statuses[] = {"HUNGRY", "MATURE", "REPRODUCT", "WAIT"};
	if (!env.population.empty())
	fnt->printf(605, 5, HGETEXT_LEFT, 
	"FPS: %d "
	"\nPopulation: %d"
	"\nStep: %d"
	"\nTESTVAL:%d",
	hge->Timer_GetFPS(), 
	env.population.size(), 
	(int) env.stepCount,
	testVal 
	);
	
	winManager->Render();

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
	objsColor = 0xFF111177;

	closeButton = getButtonTex(15, 15, objsColor);

	fnt = new hgeFont("123.fnt");
	fnt->SetScale(0.5);

	mainGUI=new hgeGUI();
	CreateGUI();

	winManager = new GUI_win_manager();
	CreateWinManager();

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
}

void DoneEditor() {
	delete fnt;

	hge->Texture_Free(closeButton);
}

void CreateGUI() {
	hgeGUIButton *button;
	hgeGUISlider *slider;
	hgeGUIText	 *text;

	HTEXTURE butTex = getButtonTex(150, 20, objsColor);

	button=new hgeGUIButton(CMD_WIN_S_L, 620, 400, 150, 20, butTex, 0, 0);
	button->SetMode(false);
	mainGUI->AddCtrl(button);

	text = new hgeGUIText(GUI_TEXT, 620, 400, 150, 30, fnt);
	text->SetMode(HGETEXT_CENTER);
	text->SetColor(0xFFFFFFFF);
	text->SetText("Save/Load");
	text->bEnabled = false;
	mainGUI->AddCtrl(text);

	button=new hgeGUIButton(CMD_WIN_ADD_IND, 620, 430, 150, 20, butTex, 0, 0);
	button->SetMode(false);
	mainGUI->AddCtrl(button);

	text = new hgeGUIText(GUI_TEXT, 620, 430, 150, 30, fnt);
	text->SetMode(HGETEXT_CENTER);
	text->SetColor(0xFFFFFFFF);
	text->SetText("Add Individ");
	text->bEnabled = false;
	mainGUI->AddCtrl(text);
}

void CreateWinManager() {
	RGBColor colPix(0xFF00DD00);

	HTEXTURE butTex = getButtonTex(80,15,objsColor);
	hgeGUIButton *button;
	button = new hgeGUIButton(0, 0, 0, 80, 15, butTex, 0, 0);
	button->SetMode(false);

	////////////////////////////////////////////////////
	////////////////WINDOW SAVE AND LOAD////////////////
	////////////////////////////////////////////////////
	GUI_window *winSaveLoad;
	winSaveLoad = new GUI_window(200, 200, "Save/Load", fnt, 0xFF999999, 0xFF666666, &closeButton);

	hgeGUIText *buttonText;
	buttonText = new hgeGUIText(GUI_TEXT, 0, 0, 80, 15, fnt);
	buttonText->SetMode(HGETEXT_CENTER);
	buttonText->SetColor(0xFFFFFFFF);
	buttonText->bEnabled = false;

	hgeGUIListbox *slotList;
	slotList = new hgeGUIListbox(0, 0, 0, 200, 170, fnt, 0xFFFFFFFF, 0xFFFFFFFF, objsColor);
	slotList->AddItem("Slot 1");
	slotList->AddItem("Slot 2");
	slotList->AddItem("Slot 3");
	slotList->AddItem("Slot 4");
	slotList->AddItem("Slot 5");
	slotList->AddItem("Slot 6");
	winSaveLoad->addCtrl(slotList, 0, 20, "slot_list");

	buttonText->SetText("Save");
	winSaveLoad->addCtrl(button, 10, 180, "but_save", save_func);
	winSaveLoad->addCtrl(buttonText, 10, 185, "but_save_text");
	buttonText->SetText("Load");
	winSaveLoad->addCtrl(button, 110, 180, "but_load", load_func);
	winSaveLoad->addCtrl(buttonText, 110, 185, "but_load_text");

	winManager->addWindow(winSaveLoad, WIN_S_L);
	winManager->setWinPos(WIN_S_L, 200, 200);
	////////////////////////////////////////////////////
	////////////////WINDOW ADD INDIVID//////////////////
	////////////////////////////////////////////////////
	GUI_window *winAddIndivid;
	winAddIndivid = new GUI_window(340, 285, "Add individ", fnt, 0xFF999999, 0xFF666666, &closeButton);

	hgeGUIListbox *stateList;
	stateList = new hgeGUIListbox(0, 0, 0, 100, 100, fnt, 0xFFFFFFFF, 0xFFFFFFFF, objsColor);
	stateList->AddItem("HUNGRY");
	stateList->AddItem("MATURE");
	stateList->AddItem("REPRODUCT");
	stateList->AddItem("WAIT");
	winAddIndivid->addCtrl(stateList, 0, 50, "state_list");

	sliderTexture = new Pixel(objsColor);
	hgeGUISlider *indSlider;
	indSlider = new hgeGUISlider(GUI_SLIDER, 0, 0, 200, 10, sliderTexture->getTex(), 0, 0, 1, 1);
	indSlider->SetMode(-20, 20, HGESLIDER_BARRELATIVE);
	indSlider->SetValue(10);
	
	winAddIndivid->addCtrl(indSlider, 120, 35, "max_speed_s");
	winAddIndivid->addCtrl(indSlider, 120, 60, "rand_way_s");
	winAddIndivid->addCtrl(indSlider, 120, 85, "partner_s");
	winAddIndivid->addCtrl(indSlider, 120, 110, "cohesion_partner_s");
	winAddIndivid->addCtrl(indSlider, 120, 135, "separation_partner_s");
	winAddIndivid->addCtrl(indSlider, 120, 160, "alignment_partner_s");
	winAddIndivid->addCtrl(indSlider, 120, 185, "enemy_s");
	winAddIndivid->addCtrl(indSlider, 120, 210, "cohesion_enemy_s");
	winAddIndivid->addCtrl(indSlider, 120, 235, "separation_enemy_s");
	winAddIndivid->addCtrl(indSlider, 120, 260, "alignment_enemy_s");

	hgeGUIText *slidersStaticText;
	slidersStaticText = new hgeGUIText(0, 0, 0, 100, 20, fnt);
	slidersStaticText->SetMode(HGETEXT_LEFT);
	slidersStaticText->bEnabled = false;

	slidersStaticText->SetText("Max speed:");
	winAddIndivid->addCtrl(slidersStaticText, 120, 25, "max_speed_t");
	slidersStaticText->SetText("Rand way:");
	winAddIndivid->addCtrl(slidersStaticText, 120, 50, "rand_way_t");
	slidersStaticText->SetText("Partners effect:");
	winAddIndivid->addCtrl(slidersStaticText, 120, 75, "partner_t");
	slidersStaticText->SetText("Partners cohesion:");
	winAddIndivid->addCtrl(slidersStaticText, 120, 100, "cohesion_partner_t");
	slidersStaticText->SetText("Partners separation:");
	winAddIndivid->addCtrl(slidersStaticText, 120, 125, "separation_partner_t");
	slidersStaticText->SetText("Partners alignment:");
	winAddIndivid->addCtrl(slidersStaticText, 120, 150, "alignment_partner_t");
	slidersStaticText->SetText("Enemies effect:");
	winAddIndivid->addCtrl(slidersStaticText, 120, 175, "enemy_t");
	slidersStaticText->SetText("Enemies cohesion:");
	winAddIndivid->addCtrl(slidersStaticText, 120, 200, "cohesion_enemy_t");
	slidersStaticText->SetText("Enemies separation:");
	winAddIndivid->addCtrl(slidersStaticText, 120, 225, "separation_enemy_t");
	slidersStaticText->SetText("Enemies alignment:");
	winAddIndivid->addCtrl(slidersStaticText, 120, 250, "alignment_enemy_t");

	
	hgeGUIText *slidersValText;
	slidersValText = new hgeGUIText(0, 0, 0, 100, 20, fnt);
	slidersValText->SetMode(HGETEXT_RIGHT);
	slidersValText->bEnabled = false;
	
	slidersValText->SetText("0");
	winAddIndivid->addCtrl(slidersValText, 220, 25, "max_speed_val");
	slidersValText->SetText("0");
	winAddIndivid->addCtrl(slidersValText, 220, 50, "rand_way_val");
	slidersValText->SetText("0");
	winAddIndivid->addCtrl(slidersValText, 220, 75, "partner_val");
	slidersValText->SetText("0");
	winAddIndivid->addCtrl(slidersValText, 220, 100, "cohesion_partner_val");
	slidersValText->SetText("0");
	winAddIndivid->addCtrl(slidersValText, 220, 125, "separation_partner_val");
	slidersValText->SetText("0");
	winAddIndivid->addCtrl(slidersValText, 220, 150, "alignment_partner_val");
	slidersValText->SetText("0");
	winAddIndivid->addCtrl(slidersValText, 220, 175, "enemy_val");
	slidersValText->SetText("0");
	winAddIndivid->addCtrl(slidersValText, 220, 200, "cohesion_enemy_val");
	slidersValText->SetText("0");
	winAddIndivid->addCtrl(slidersValText, 220, 225, "separation_enemy_val");
	slidersValText->SetText("0");
	winAddIndivid->addCtrl(slidersValText, 220, 250, "alignment_enemy_val");

	winManager->addWindow(winAddIndivid, WIN_ADD_IND);
	winManager->setWinPos(WIN_ADD_IND, 200, 200);
	////////////////////////////////////////////////////
	////////////////WINDOW EDIT PHIS////////////////////
	////////////////////////////////////////////////////
}