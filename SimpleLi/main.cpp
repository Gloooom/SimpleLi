
#include "stdafx.h"


#pragma comment (lib, "hge.lib")
#pragma comment (lib, "hgehelp.lib")

#pragma once

#include "Logic\Individ.h"
#include "Logic\Environment.h"
#include "Logic\Types.h"

HGE *hge=0;


hgeFont			*fnt;
hgeGUI			*gui;

HTEXTURE		texGui, texBut, texCell;

hgeSprite		*sprLeftPane1, *sprLeftPane2, *sprRightPane1, *sprRightPane2;
hgeSprite		*sprCell;

PEditorState	state;

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

	if(HandleKeys(hge->Input_GetKey())) return true;

	if(DoCommands(gui->Update(dt))) return true;

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



	for (int x=0; x<W; x++) {
		for(int y=0; y<H; y++) {
			//hgeColorRGB col(hge->Random_Float(0, 1),hge->Random_Float(0,1),hge->Random_Float(0,1), 1);
			sprCell->SetColor(Field[x][y].color); 
			sprCell->RenderStretch(Field[x][y].x, Field[x][y].y, Field[x][y].x+Field[x][y].width, Field[x][y].y+Field[x][y].height);
		}
	}

	std::map <long long int, Individ> ::iterator p = env.population.begin();
	while (p != env.population.end()) {
		Cell c = Field[p->second.pos.x][p->second.pos.y];
		float x1, y1, x2, y2;
		x1 = c.x+c.width/2;
		y1 = c.y+c.height/2;
		x2 = x1-p->second.way.x*20;
		y2 = y1-p->second.way.y*20;
		Line l(x1, y1, x2, y2, 5, 0x55009999);
		l.RenderLine();
		p++;
	}

	//gui->Render();

	std::string statuses[] = {"HUNGRY", "EAT", "MATURE", "REPRODUCT", "WAIT"};
	if (!env.population.empty())
	fnt->printf(605, 5, HGETEXT_LEFT, 
	"FPS: %d \nPopulation: %d\n\n\n"
	"\nlive: %d"
	"\nhp: %d"
	"\nenergy: %d"
	"\nspeed: %f"
	"\n\nlive_timer: %d"
	"\nreproduction_timer: %d"
	"\nstate: %s",
		hge->Timer_GetFPS(), env.population.size(), 
		selectInd->live,
		selectInd->hp,
		selectInd->energy,
		selectInd->speed,
		selectInd->live_timer,
		selectInd->reproduction_timer,
		statuses[selectInd->state].c_str());
		
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
	FOV eye1, eye2;
	eye1.height=30; eye2.height=3;
	eye1.width=20; eye2.width=30;
	eye1.angle=0; eye2.angle=0;
	eye1.type=TRIANGLE; eye2.type=RADIUS;
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
		g.eyes.push_back(eye1);
		g.eyes.push_back(eye2);	
		g.diet=diet;
		for (int i=0; i<end_of_status; i++) {
	if (diet==AUTO) {
		g.soc[i][max_speed] = 2;
		g.soc[i][rand_way] = 1;
		g.soc[i][partner] = 1;
		g.soc[i][cohesion_partner] = 1;
		g.soc[i][separation_partner] = 0.5f;
		g.soc[i][alignment_partner] = 1.5f;
		g.soc[i][enemy] = 1;
		g.soc[i][cohesion_enemy] = 0;
		g.soc[i][separation_enemy] = 8;
		g.soc[i][alignment_enemy] = 0.0f;
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
	texCell=hge->Texture_Load("cell.png");

	fnt=new hgeFont("123.fnt");
	fnt->SetScale(0.5);
	sprCell=new hgeSprite(texCell, 0, 0, 32, 32);
	sprCell->SetHotSpot(0, 0);

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

	gui=new hgeGUI();
	CreateGUI();
}

void DoneEditor() {
	delete gui;
	delete fnt;
	delete sprCell;

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
}