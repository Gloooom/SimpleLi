#include "Main_inclusion.h"

#pragma comment (lib, "hge.lib")
#pragma comment (lib, "hgehelp.lib")

#pragma once

#include "GUI\Gui_graphic_area.h"
#include "Logic\Environment.h"

#include "GUI\Gui_win_manager.h"



#define POP_A 50
#define POP_G 0

HGE *hge=0;

Environment env(100, 100);
EditorState	state;

hgeGUI			*mainGUI;
GUI_win_manager *winManager;
GraphicArea		*display;

RGBColor		objsColor;
hgeFont			*fnt;
Pixel			*sliderTexture;
hgeSprite		*testSpr;

#include "GUI_structure.h"

Individ *selectInd=&(env.empty);

float			timer = 0;
int				mp_x = 0;
int				mp_y = 0;

void			InitEditor();
void			DoneEditor();
void			InitEnvironment();
void			addIndivid(Point <float> p, Mode_feeding diet); 

bool FrameFunc()
{
	float		dt=hge->Timer_GetDelta();
	
	hge->Input_GetMousePos(&state.mp.x, &state.mp.y);

	winManager->Update(dt, state.mp.x, state.mp.y);
	display->getMousePos(state.mp.x, state.mp.y, &mp_x, &mp_y);

	mainGUI->Update(dt);

	CheckButtons();

	if (state.play) {
		timer+=dt;
		if (timer>0.1) {
			env.step();
			display->Clear();
			for (int x=0; x<env.W(); x++)
				for (int y=0; y<env.H(); y++) {
					if (env.field(x, y)->gender == MALE)
						(*display)(x, y) = 0xFF990000;
					if (env.field(x, y)->gender == FEMALE)
						(*display)(x, y) = 0xFF000099;
				}
			display->Update();
			timer=0;
		}
	}

	return false;
}


bool RenderFunc()
{
	hge->Gfx_Clear(0);
	hge->Gfx_BeginScene();
	
	if (!env.population.empty())
		fnt->printf(605, 5, HGETEXT_LEFT, 
		"FPS: %d "
		"\nPopulation: %d"
		"\nStep: %d"
		"\nMousePos:%d  %d",
		hge->Timer_GetFPS(), 
		env.population.size(), 
		(int) env.stepCount,
		mp_x,
		mp_y
		);

	display->Render();

	std::map <long long int, Individ> ::iterator p = env.population.begin();
	while (p != env.population.end()) {
		Cell c = (*display)[p->second.pos.x + p->second.pos.y*env.W()];
		float x1, y1, x2, y2;
		x1 = (c.getQuad()->v[0].x +  c.getQuad()->v[2].x)/2;
		y1 = (c.getQuad()->v[0].y +  c.getQuad()->v[2].y)/2;
		x2 = x1 + p->second.way.x*20;
		y2 = y1 + p->second.way.y*20;
		hge->Gfx_RenderLine(x1, y1, x2, y2, 0xAA00AA00);
		p++;
	}
	
	mainGUI->Render();
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
		g.radialEye.setHeight(0);
		for(int i = func::randi(1, 3); i>=0; i--)
			g.eyes.push_back(FOV_Tri(func::randf(-M_PI, M_PI), func::randi(5, 40), func::randi(5, 40)));
		g.diet=diet;
		g.color = 0xFF000000;
		for (int i=0; i<end_of_status; i++) {
			if (diet==AUTO) {
				g.soc[i][max_speed] = func::randf(0, 5);
				g.soc[i][libido] = func::randf(0, 10);
				g.soc[i][rand_way] = func::randf(0, M_PI*2);
				g.soc[i][partner] = func::randf(0, 10);
				g.soc[i][cohesion_partner] = func::randf(0, 10);
				g.soc[i][separation_partner] = func::randf(0, 10);
				g.soc[i][alignment_partner] = func::randf(0, 10);
				g.soc[i][enemy] = func::randf(0, 10);
				g.soc[i][cohesion_enemy] = func::randf(0, 10);
				g.soc[i][separation_enemy] = func::randf(0, 10);
				g.soc[i][alignment_enemy] = func::randf(0, 10);
				/*g.soc[i][max_speed] = 2;
				g.soc[i][libido] = 1;
				g.soc[i][rand_way] = 1;
				g.soc[i][partner] = 1;
				g.soc[i][cohesion_partner] = 1;
				g.soc[i][separation_partner] = 0.2;
				g.soc[i][alignment_partner] = 0.1;
				g.soc[i][enemy] = 1;
				g.soc[i][cohesion_enemy] = 1;
				g.soc[i][separation_enemy] = 1;
				g.soc[i][alignment_enemy] = 1;*/
			}
		}
		g.soc[0][libido] = 0;
		g.soc[2][libido] = 0;
		g.color = 0xFF009900;
		env.addIndivid(Individ(p.round(), g));
}

void InitEnvironment() {
	//env.setMutation(1, 0.1, 0.2, 0.1, ONE);
	env.setMutation(0, 0, 0, 0, ONE);
	Point <float> p;

	for(int i=0; i<POP_A; i++) {
		p.x=func::randi(0, env.W()-2);
		p.y=func::randi(0, env.H()-2); 
		addIndivid(p, AUTO);
	}
	for(int i=0; i<POP_G; i++) {
		p.x=func::randi(0, env.W()-2);
		p.y=func::randi(0, env.H()-2); 
		addIndivid(p, GETERO);
	}
	
	env.step();
	display->Update();
}

void InitEditor() {
	objsColor = 0xFF111177;

	fnt = new hgeFont("123.fnt");
	fnt->SetScale(0.5);

	mainGUI=new hgeGUI();
	CreateMainGUI();

	winManager = new GUI_win_manager();
	CreateWinManager();

	display = new GraphicArea(env.W(), env.H(), 600, 600);
	display->setBorder(1);
	display->setVisibleArea(0, 0, env.W(), env.H());
}

void DoneEditor() {
	//delete fnt;
}
