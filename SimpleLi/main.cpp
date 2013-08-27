
#include "stdafx.h"


#pragma comment (lib, "hge.lib")
#pragma comment (lib, "hgehelp.lib")

#pragma once

#include "Logic\Individ.h"
#include "Logic\Environment.h"
#include "Logic\Types.h"
#include "Gui_window.h"

HGE *hge=0;


hgeFont			*fnt;
hgeGUI			*gui;

HTEXTURE		texGui, texBut, texCell;

hgeSprite		*sprLeftPane1, *sprLeftPane2, *sprRightPane1, *sprRightPane2;

PEditorState	state;

GUI_window *test_w;

Cell Field[W][H];
double border=0;

Environment env;
Individ *selectInd=&(env.empty);

float			psx=400, psy=300;
float			timer=0;
bool			play=true;

void			InitEditor();
void			DoneEditor();
void			CreateGUI();
void			InitEnvironment();
void			addIndivid(Point <float> p, Mode_feeding diet); 

#include "Gui_functions.h"

bool FrameFunc()
{
	float		px, py;
	float		dt=hge->Timer_GetDelta();
	
	hge->Input_GetMousePos(&state.mp.x, &state.mp.y);

	//if(HandleKeys(hge->Input_GetKey())) return true;

	//if(DoCommands(gui->Update(dt))) return true;
	test_w->Update(dt, state.mp.x, state.mp.y);

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
			//Vector <int> tempP;
			//tempP.x=state.mp.x/(Field[0][0].width+border);
			//tempP.y=state.mp.y/(Field[0][0].height+border);
			//Vector <double> v (tempP.x - env.population[0].pos.x, tempP.y - env.population[0].pos.y);
			//env.population[0].way=v.getNorm();
			//env.population[0].move(&env.field, env.empty);
			


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

	
	//gui->Render();
	test_w->Render();

	std::string statuses[] = {"HUNGRY", "EAT", "MATURE", "REPRODUCT", "WAIT"};
	if (!env.population.empty())
	fnt->printf(605, 5, HGETEXT_LEFT, 
	"FPS: %d "
	"\nPopulation: %d"
	"\nStep: %d\n\n\n\n"
	"\nlive: %d"
	"\nhp: %d"
	"\nenergy: %d"
	"\nspeed: %f"
	"\n\nlive_timer: %d"
	"\nreproduction_timer: %d",
	hge->Timer_GetFPS(), 
	env.population.size(), 
	(int) env.stepCount,
	selectInd->live,
	selectInd->hp,
	selectInd->energy,
	selectInd->speed,
	(int) selectInd->live_timer,
	selectInd->reproduction_timer
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

	//gui=new hgeGUI();
	//CreateGUI();
	test_w = new GUI_window(hge, 10, 10, 200, 200, "OLOLO", fnt, 0xFFAAAAAA, 0xFF999999, &texCell);
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

	gui->AddCtrl(new hgeGUIButton(CMD_EXIT, 0, 0, 73, 17, texBut, 0, 0));
	button=new hgeGUIButton(CMD_HELP, 0, 18, 73, 17, texBut, 0, 0);
	button->SetMode(true);
	gui->AddCtrl(button);

	slider = new hgeGUISlider(GUI_SLIDER, 610, 400, 180, 10, texCell, 0, 0, 30, 30);
	slider->SetMode(0, 100, HGESLIDER_BAR);
	slider->SetValue(1);
	gui->AddCtrl(slider);

	text=new hgeGUIText(GUI_TEXT, 610, 390, 180, 10, fnt);
	text->SetMode(HGETEXT_LEFT);
	text->SetColor(0xFFFFFFFF);
	text->SetText("O");
	gui->AddCtrl(text);
	text->SetText("GG");
	//GetTextCtrl(GUI_TEXT)->printf("OLOLOLO");
}