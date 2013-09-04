
#include "Main_inclusion.h"

#pragma comment (lib, "hge.lib")
#pragma comment (lib, "hgehelp.lib")

#pragma once

#include "GUI\Gui_graphic_area.h"
#include "Logic\Environment.h"

#include "GUI\Gui_win_manager.h"


#define GUI_TEXT				0

#define CMD_WIN_S_L				1
#define CMD_WIN_ADD_IND			2
#define CMD_WIN_EDIT_PHIS		3
#define CMD_PAUSE				4

#define WIN_S_L					1
#define WIN_ADD_IND				2
#define WIN_EDIT_EYE			3
#define WIN_EDIT_PHIS			4

HGE *hge=0;

Environment env;
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
int				testVal = 0;

void			InitEditor();
void			DoneEditor();
void			InitEnvironment();
void			addIndivid(Point <float> p, Mode_feeding diet); 

bool FrameFunc()
{
	float		dt=hge->Timer_GetDelta();
	
	hge->Input_GetMousePos(&state.mp.x, &state.mp.y);

	winManager->Update(dt, state.mp.x, state.mp.y);

	mainGUI->Update(dt);

	if (hgeButtonGetState(mainGUI, CMD_WIN_S_L)) {
		winManager->Activate(WIN_S_L);
		winManager->setFocus(WIN_S_L);
	}
	if (hgeButtonGetState(mainGUI, CMD_WIN_ADD_IND)) {
		winManager->Activate(WIN_ADD_IND);
		winManager->setFocus(WIN_ADD_IND);
	}
	if (hgeButtonGetState(mainGUI, CMD_WIN_EDIT_PHIS)) {
		winManager->Activate(WIN_EDIT_PHIS);
		winManager->setFocus(WIN_EDIT_PHIS);
	}
	if (hgeButtonGetState(mainGUI, CMD_PAUSE)) {
		state.play = false;
	} else state.play = true;

	if (state.play) {
		timer+=dt;
		if (timer>0.1) {
			env.step();
			display->Clear();
			for (int x=0; x<W; x++)
				for (int y=0; y<H; y++) {
					if (env.field[x][y]->gender == MALE)
						(*display)(x, y) = 0xFF990000;
					if (env.field[x][y]->gender == FEMALE)
						(*display)(x, y) = 0xFF000099;
				}
			display->Update();
			timer=0;
		}
	}

	//заготовка дл€ метода look индивида.
	//кака€-то странан€ херн€ с поворотом шаблона. ¬озможно дело в округлении точек при повороте. ј возможно и нет.
	//ƒобавить красивые затухающие окошки об каких-то событи€х в центре экрана. Ѕольше кава€, да!
	Point <int> pos(35, 35);

	std::vector <hgeQuad> ::iterator p = winManager->getWin(WIN_EDIT_EYE)->graphic.begin();
	while(p != winManager->getWin(WIN_EDIT_EYE)->graphic.end()) {
		setQuadColor(&*p, objsColor);
		p++;
	}

	std::vector <Vector <double> > mem;

	FOV_Tri eee(0, (GetWinSliderValue(winManager, WIN_EDIT_EYE, "height_s")), 
					(GetWinSliderValue(winManager, WIN_EDIT_EYE, "width_s")));
	double wayAngle = 0; //(GetWinSliderValue(winManager, WIN_EDIT_EYE, "angle_s")/100)*M_PI;
	double k1=tan(eee.angle() + atan(eee.height()/(eee.width()/2)) - wayAngle), b1=0;
	double k2=tan(eee.angle() - atan(eee.height()/(eee.width()/2)) - wayAngle), b2=0;
	double k3=tan(eee.angle() - wayAngle),    b3=(eee.height()/cos(eee.angle() - wayAngle));

	Point <double> vert[3];
	vert[0]=func::crossLine(k1,b1,k2,b2);
	vert[1]=func::crossLine(k2,b2,k3,b3);
	vert[2]=func::crossLine(k1,b1,k3,b3);

	Vector <double> vectorR(vert[2].x, vert[2].y);
	double R = vectorR.getLength();

	Vector <double> P;
	int start_x = (pos.x-R>0) ? pos.x-R : 0;
	int start_y = (pos.y-R>0) ? pos.y-R : 0;

	for (int _x=start_x; _x<pos.x+R, _x<W; _x++) { 
		P.x=_x-pos.x;
		for (int _y=start_y; _y<pos.y+R, _y<H; _y++) {
			P.y=_y-pos.y;
			double pl1, pl2, pl3;
			pl1 = (vert[0].x - P.y)*(vert[1].y - vert[0].y)-(vert[1].x - vert[0].x)*(vert[0].y - P.x);
			pl2 = (vert[1].x - P.y)*(vert[2].y - vert[1].y)-(vert[2].x - vert[1].x)*(vert[1].y - P.x);
			pl3 = (vert[2].x - P.y)*(vert[0].y - vert[2].y)-(vert[0].x - vert[2].x)*(vert[2].y - P.x);
			if ((pl1 >= 0 && pl2 >= 0 && pl3 >= 0) || (pl1 <= 0 && pl2 <= 0 && pl3 <= 0)) {
				Point <int> absP;
				absP.x=func::round(P.x+pos.x);
				absP.y=func::round(P.y+pos.y);

				mem.push_back(Vector <double> (P.x, P.y));
			}
		}
	}

	std::vector <Vector <int> > mem_2;
	std::vector <Vector <double> > ::iterator m = mem.begin();
	while (m != mem.end()) {
		m->rotate((GetWinSliderValue(winManager, WIN_EDIT_EYE, "angle_s")/100)*M_PI*2);
		
		Vector <int> g(m->x, m->y);
		mem_2.push_back(g);
		g.x+=35;
		g.y*=(-1);
		g.y+=35;
		
		if (g.x + g.y*70 >= 0 && g.x + g.y*70 < 70*70) 
			setQuadColor(&(winManager->getWin(WIN_EDIT_EYE)->graphic[g.x + g.y*70]), 0xFFFF0000);
		m++;
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
		"\nTESTVAL:%d",
		hge->Timer_GetFPS(), 
		env.population.size(), 
		(int) env.stepCount,
		testVal 
		);

	
	display->Render();

	std::map <long long int, Individ> ::iterator p = env.population.begin();
	while (p != env.population.end()) {
		Cell c = (*display)[p->second.pos.x + p->second.pos.y*W];
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
		g.radialEye.setHeight(3);
		g.eyes.push_back(FOV_Tri(0, 30, 20));
		g.diet=diet;
		g.color = 0xFF000000;
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
		}
		g.color = 0xFF009900;
		env.addIndivid(Individ(p.round(), g));
}

void InitEnvironment() {
	env.setMutation(1, 0.1, 0.2, 0, ONE);

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

	fnt = new hgeFont("123.fnt");
	fnt->SetScale(0.5);

	mainGUI=new hgeGUI();
	CreateGUI();

	winManager = new GUI_win_manager();
	CreateWinManager();

	display = new GraphicArea(W, H, 600, 600);
	display->setBorder(1);
	display->setVisibleArea(0, 0, W, H);
}

void DoneEditor() {
	delete fnt;
}
