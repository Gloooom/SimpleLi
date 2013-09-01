
#include "Main_inclusion.h"

#pragma comment (lib, "hge.lib")
#pragma comment (lib, "hgehelp.lib")

#pragma once

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


hgeFont			*fnt;
hgeGUI			*mainGUI;

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
	case 0: env.save("save1.bin"); break;
	case 1: env.save("save2.bin"); break;
	case 2: env.save("save3.bin"); break;
	case 3: env.save("save4.bin"); break;
	case 4: env.save("save5.bin"); break;
	case 5: env.save("save6.bin"); break;
	}
};

void load_func() {
	int selectSlot = GetWinListboxSelect(winManager, WIN_S_L, "slot_list");
	switch(selectSlot) {
	case 0: env.load("save1.bin"); break;
	case 1: env.load("save2.bin"); break;
	case 2: env.load("save3.bin"); break;
	case 3: env.load("save4.bin"); break;
	case 4: env.load("save5.bin"); break;
	case 5: env.load("save6.bin"); break;
	}
};

void test_func() {
	testVal++;
};


void call_EyeEdit() {
	winManager->Activate(WIN_EDIT_EYE);
	winManager->setFocus(WIN_EDIT_EYE);
}

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
			timer=0;
		}
	}


	//заготовка для метода look индивида.
	//какая-то странаня херня с поворотом шаблона. Возможно дело в округлении точек при повороте. А возможно и нет.
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

	testVal = 0;
	std::vector <hgeQuad> ::iterator t = winManager->getWin(WIN_EDIT_EYE)->graphic.begin();
	while (t != winManager->getWin(WIN_EDIT_EYE)->graphic.end()) {
		if (t->v[0].col == 0xFFFF0000) testVal++;
		t++;
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
		RGBColor c(func::randf(0,1), func::randf(0,1), func::randf(0,1), 1);
		g.color = c;
		env.addIndivid(Individ(p.round(), g));
}

void InitEnvironment() {
	env.setMutation(1, 1, 1, 0.1, ONE);

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



	Cell typicCell;
	RGBColor color(1, 1, 1, 1);
	typicCell.color=color;
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
}

void CreateGUI() {
	hgeGUIButton *button;
	hgeGUIText	 *text;

	HTEXTURE butTex = getButtonTex(180, 20, objsColor, 0.1);

	button=new hgeGUIButton(CMD_WIN_S_L, 610, 400, 180, 20, butTex, 0, 0);
	button->SetMode(false);
	mainGUI->AddCtrl(button);

	text = new hgeGUIText(GUI_TEXT, 610, 400, 180, 30, fnt);
	text->SetMode(HGETEXT_CENTER);
	text->SetColor(0xFFFFFFFF);
	text->SetText("Save/Load");
	text->bEnabled = false;
	mainGUI->AddCtrl(text);

	button=new hgeGUIButton(CMD_WIN_ADD_IND, 610, 430, 180, 20, butTex, 0, 0);
	button->SetMode(false);
	mainGUI->AddCtrl(button);

	text = new hgeGUIText(GUI_TEXT, 610, 430, 180, 30, fnt);
	text->SetMode(HGETEXT_CENTER);
	text->SetColor(0xFFFFFFFF);
	text->SetText("Add Individ");
	text->bEnabled = false;
	mainGUI->AddCtrl(text);

	button=new hgeGUIButton(CMD_WIN_EDIT_PHIS, 610, 460, 180, 20, butTex, 0, 0);
	button->SetMode(false);
	mainGUI->AddCtrl(button);

	text = new hgeGUIText(GUI_TEXT, 610, 460, 180, 30, fnt);
	text->SetMode(HGETEXT_CENTER);
	text->SetColor(0xFFFFFFFF);
	text->SetText("Edit phis attributes");
	text->bEnabled = false;
	mainGUI->AddCtrl(text);

	button=new hgeGUIButton(CMD_PAUSE, 610, 560, 180, 20, butTex, 0, 0);
	button->SetMode(true);
	button->SetState(true);
	mainGUI->AddCtrl(button);

	text = new hgeGUIText(GUI_TEXT, 610, 560, 180, 30, fnt);
	text->SetMode(HGETEXT_CENTER);
	text->SetColor(0xFFFFFFFF);
	text->SetText("Pause");
	text->bEnabled = false;
	mainGUI->AddCtrl(text);
}

void CreateWinManager() {
	RGBColor backCol(0xFF666666);
	RGBColor headCol(0xFF999999);
	RGBColor colPix(0xFF00DD00);
	//sliderTexture = new Pixel(objsColor);
	HTEXTURE *sliderTex = new HTEXTURE(getButtonTex(10, 10, 0xFFFFFFFF, 0.1));

	//Такой слайдер гораздо лучше
	//HTEXTURE *tex = new HTEXTURE(getButtonTex(10, 10, 0xFFFFFFFF, 0.1));
	//slid = new hgeGUISlider(0, 0, 0, 200, 10, *tex, 1, 0, 5, 10);

	////////////////////////////////////////////////////
	//////////////////////WINDOWS///////////////////////
	////////////////////////////////////////////////////
	////////////////WINDOW SAVE AND LOAD////////////////
	////////////////////////////////////////////////////
	GUI_window *winSaveLoad;
	winSaveLoad = new GUI_window(200, 200, "Save/Load", fnt, headCol, backCol, objsColor);

	HTEXTURE butSVTex = getButtonTex(80, 15, objsColor, 0.15);
	hgeGUIButton *butSV;
	butSV = new hgeGUIButton(0, 0, 0, 80, 15, butSVTex, 0, 0);

	hgeGUIText *butSVText;
	butSVText = new hgeGUIText(GUI_TEXT, 0, 0, 80, 15, fnt);
	butSVText->SetMode(HGETEXT_CENTER);
	butSVText->SetColor(0xFFFFFFFF);
	butSVText->bEnabled = false;
	
	butSVText->SetText("Save");
	winSaveLoad->addCtrl(butSV, 10, 180, "but_save", save_func);
	winSaveLoad->addCtrl(butSVText, 10, 185, "but_save_text");
	butSVText->SetText("Load");
	winSaveLoad->addCtrl(butSV, 110, 180, "but_load", load_func);
	winSaveLoad->addCtrl(butSVText, 110, 185, "but_load_text");

	hgeGUIListbox *slotList;
	slotList = new hgeGUIListbox(0, 0, 0, 200, 150, fnt, 0xFFFFFFFF, 0xFFFFFFFF, objsColor);
	slotList->AddItem("Slot 1");
	slotList->AddItem("Slot 2");
	slotList->AddItem("Slot 3");
	slotList->AddItem("Slot 4");
	slotList->AddItem("Slot 5");
	slotList->AddItem("Slot 6");
	winSaveLoad->addCtrl(slotList, 0, 20, "slot_list");

	winManager->addWindow(winSaveLoad, WIN_S_L);
	////////////////////////////////////////////////////
	////////////////WINDOW ADD INDIVID//////////////////
	////////////////////////////////////////////////////
	GUI_window *winAddIndivid;
	winAddIndivid = new GUI_window(340, 285, "Add individ", fnt, headCol, backCol, objsColor);

	hgeGUIListbox *stateList;
	stateList = new hgeGUIListbox(0, 0, 0, 100, 100, fnt, 0xFFFFFFFF, 0xFFFFFFFF, objsColor);
	stateList->AddItem("HUNGRY");
	stateList->AddItem("MATURE");
	stateList->AddItem("REPRODUCT");
	stateList->AddItem("WAIT");
	winAddIndivid->addCtrl(stateList, 0, 50, "state_list");

	hgeGUISlider *indSlider;
	indSlider = new hgeGUISlider(0, 0, 0, 200, 10, *sliderTex, 1, 0, 5, 10);
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
	winAddIndivid->addCtrl(slidersValText, 220, 50, "rand_way_val");
	winAddIndivid->addCtrl(slidersValText, 220, 75, "partner_val");
	winAddIndivid->addCtrl(slidersValText, 220, 100, "cohesion_partner_val");
	winAddIndivid->addCtrl(slidersValText, 220, 125, "separation_partner_val");
	winAddIndivid->addCtrl(slidersValText, 220, 150, "alignment_partner_val");
	winAddIndivid->addCtrl(slidersValText, 220, 175, "enemy_val");
	winAddIndivid->addCtrl(slidersValText, 220, 200, "cohesion_enemy_val");
	winAddIndivid->addCtrl(slidersValText, 220, 225, "separation_enemy_val");
	winAddIndivid->addCtrl(slidersValText, 220, 250, "alignment_enemy_val");

	HTEXTURE callEyeEditButTex = getButtonTex(100, 20, objsColor, 0.15);
	hgeGUIButton *callEyeEditBut;
	callEyeEditBut = new hgeGUIButton(0, 0, 0, 100, 20, callEyeEditButTex, 0, 0);
	winAddIndivid->addCtrl(callEyeEditBut, 10, 255, "edit_eyes_but", call_EyeEdit);

	hgeGUIText *callEyeEditButText;
	callEyeEditButText = new hgeGUIText(0, 0, 0, 100, 20, fnt);
	callEyeEditButText->SetMode(HGETEXT_CENTER);
	callEyeEditButText->bEnabled = false;
	callEyeEditButText->SetText("Edit eyes");
	winAddIndivid->addCtrl(callEyeEditButText, 10, 260, "edit_eyes_but_t");

	winManager->addWindow(winAddIndivid, WIN_ADD_IND);
	////////////////////////////////////////////////////
	////////////////WINDOW EDIT EYE/////////////////////
	////////////////////////////////////////////////////
	GUI_window *winEditEye;
	winEditEye = new GUI_window(370, 510, "Edit eye", fnt, headCol, backCol, objsColor);

	hgeQuad cellQ;
	RGBColor pixCol(1, 1, 0, 0);
	int areaW = 350, areaH = 350;
	int countW = 70, countH = 70;
	int b = 1;
	int w = areaW/countW - b, h = areaH/countH - b;
	int pos_x=10, pos_y=25;
	for(int y=0; y<countH; y++) {		
		for (int x=0; x<countW; x++) {
			cellQ.v[0].x=b*x+w*x+pos_x;
			cellQ.v[0].y=b*y+h*y+pos_y;

			cellQ.v[1].x=b*x+w*x+w+pos_x;
			cellQ.v[1].y=b*y+h*y+pos_y;

			cellQ.v[2].x=b*x+w*x+w+pos_x;
			cellQ.v[2].y=b*y+h*y+h+pos_y;

			cellQ.v[3].x=b*x+w*x+pos_x;
			cellQ.v[3].y=b*y+h*y+h+pos_y;
			
			cellQ.v[0].col=
			cellQ.v[1].col=
			cellQ.v[2].col=
			cellQ.v[3].col=objsColor;
			cellQ.v[0].z=
			cellQ.v[1].z=
			cellQ.v[2].z=
			cellQ.v[3].z=0.5f;

			cellQ.blend=BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE;
			cellQ.tex=0;
			
			winEditEye->graphic.push_back(cellQ);
		}
	}

	hgeGUISlider *eyeSlider;
	eyeSlider = new hgeGUISlider(0, 0, 0, 200, 10, *sliderTex, 1, 0, 5, 10);
	eyeSlider->SetMode(0, 100, HGESLIDER_BAR);
	eyeSlider->SetValue(10);
	
	winEditEye->addCtrl(eyeSlider, 10, 395, "height_rad_s");
	winEditEye->addCtrl(eyeSlider, 10, 435, "angle_s");
	winEditEye->addCtrl(eyeSlider, 10, 460, "width_s");
	winEditEye->addCtrl(eyeSlider, 10, 485, "height_s");

	slidersStaticText = new hgeGUIText(0, 0, 0, 100, 20, fnt);
	slidersValText->SetMode(HGETEXT_LEFT);
	slidersValText->bEnabled = false;
	
	slidersValText->SetText("Radial eye height:");
	winEditEye->addCtrl(slidersValText, 10, 385, "height_rad_t");
	slidersValText->SetText("Eye angle:");
	winEditEye->addCtrl(slidersValText, 10, 425, "angle_t");
	slidersValText->SetText("Eye width:");
	winEditEye->addCtrl(slidersValText, 10, 450, "width_t");
	slidersValText->SetText("Eye height:");
	winEditEye->addCtrl(slidersValText, 10, 475, "height_t");

	slidersValText = new hgeGUIText(0, 0, 0, 100, 20, fnt);
	slidersValText->SetMode(HGETEXT_RIGHT);
	slidersValText->bEnabled = false;
	slidersValText->SetText("0");

	winEditEye->addCtrl(slidersValText, 110, 385, "height_rad_val");
	winEditEye->addCtrl(slidersValText, 110, 425, "angle_val");
	winEditEye->addCtrl(slidersValText, 110, 450, "width_val");
	winEditEye->addCtrl(slidersValText, 110, 475, "height_val");

	hgeGUIListbox *eyesList;
	eyesList = new hgeGUIListbox(0, 0, 0, 30, 115, fnt, 0xFFFFFFFF, 0xFFFFFFFF, objsColor);
	eyesList->AddItem("0");
	eyesList->AddItem("1");
	eyesList->AddItem("2");
	eyesList->AddItem("3");
	eyesList->AddItem("4");
	eyesList->AddItem("5");
	winEditEye->addCtrl(eyesList, 330, 385, "eyes_list");

	HTEXTURE eyeButTex = getButtonTex(50, 50, objsColor, 0.15);
	hgeGUIButton *eyeBut;
	eyeBut = new hgeGUIButton(0, 0, 0, 50, 50, eyeButTex, 0, 0);
	winEditEye->addCtrl(eyeBut, 270, 385, "eye_but_add");
	winEditEye->addCtrl(eyeBut, 270, 450, "eye_but_del");
	
	hgeGUIText *eyeButText;
	eyeButText = new hgeGUIText(0, 0, 0, 50, 50, fnt);
	eyeButText->SetMode(HGETEXT_CENTER);
	eyeButText->bEnabled = false;
	eyeButText->SetText("Add");
	winEditEye->addCtrl(eyeButText, 270, 388, "eye_but_add_t");
	eyeButText->SetText("Del");
	winEditEye->addCtrl(eyeButText, 270, 452, "eye_but_del_t");

	winManager->addWindow(winEditEye, WIN_EDIT_EYE);
	////////////////////////////////////////////////////
	////////////////WINDOW EDIT PHIS////////////////////
	////////////////////////////////////////////////////
	GUI_window *winEditPhis;
	winEditPhis = new GUI_window(220, 370, "Edit phis attributes", fnt, headCol, backCol, objsColor);

	hgeGUISlider *phisSlider;
	phisSlider = new hgeGUISlider(0, 0, 0, 200, 10, *sliderTex, 1, 0, 5, 10);
	phisSlider->SetMode(0, 1000, HGESLIDER_BAR);
	phisSlider->SetValue(500);
	
	winEditPhis->addCtrl(phisSlider, 10, 35, "acceleration_s");
	winEditPhis->addCtrl(phisSlider, 10, 60, "hp_max_s");
	winEditPhis->addCtrl(phisSlider, 10, 85, "saturation_s");
	winEditPhis->addCtrl(phisSlider, 10, 110, "stamina_s");
	winEditPhis->addCtrl(phisSlider, 10, 135, "fertility_s");
	winEditPhis->addCtrl(phisSlider, 10, 160, "live_time_s");
	winEditPhis->addCtrl(phisSlider, 10, 185, "energy_max_s");
	winEditPhis->addCtrl(phisSlider, 10, 210, "energy_mature_s");
	winEditPhis->addCtrl(phisSlider, 10, 235, "energy_hungry_s");
	winEditPhis->addCtrl(phisSlider, 10, 260, "reproduction_cost_s");
	winEditPhis->addCtrl(phisSlider, 10, 285, "reproduction_time_s");
	winEditPhis->addCtrl(phisSlider, 10, 310, "reproduction_pause_s");

	slidersStaticText = new hgeGUIText(0, 0, 0, 100, 20, fnt);
	slidersStaticText->SetMode(HGETEXT_LEFT);
	slidersStaticText->bEnabled = false;

	slidersStaticText->SetText("Acceleration:");
	winEditPhis->addCtrl(slidersStaticText, 10, 25, "acceleration_t");
	slidersStaticText->SetText("Max HP:");
	winEditPhis->addCtrl(slidersStaticText, 10, 50, "hp_max_t");
	slidersStaticText->SetText("Saturation:");
	winEditPhis->addCtrl(slidersStaticText, 10, 75, "saturation_t");
	slidersStaticText->SetText("Stamina:");
	winEditPhis->addCtrl(slidersStaticText, 10, 100, "stamina_t");
	slidersStaticText->SetText("Fertility:");
	winEditPhis->addCtrl(slidersStaticText, 10, 125, "fertility_t");
	slidersStaticText->SetText("Live time:");
	winEditPhis->addCtrl(slidersStaticText, 10, 150, "live_time_t");
	slidersStaticText->SetText("Max energy:");
	winEditPhis->addCtrl(slidersStaticText, 10, 175, "energy_max_t");
	slidersStaticText->SetText("Mature energy:");
	winEditPhis->addCtrl(slidersStaticText, 10, 200, "energy_mature_t");
	slidersStaticText->SetText("Hungry energy:");
	winEditPhis->addCtrl(slidersStaticText, 10, 225, "energy_hungry_t");
	slidersStaticText->SetText("Reproduction cost:");
	winEditPhis->addCtrl(slidersStaticText, 10, 250, "reproduction_cost_t");
	slidersStaticText->SetText("Reproduction time:");
	winEditPhis->addCtrl(slidersStaticText, 10, 275, "reproduction_time_t");
	slidersStaticText->SetText("Reproduction pause:");
	winEditPhis->addCtrl(slidersStaticText, 10, 300, "reproduction_pause_t");

	slidersValText = new hgeGUIText(0, 0, 0, 100, 20, fnt);
	slidersValText->SetMode(HGETEXT_RIGHT);
	slidersValText->bEnabled = false;
	slidersValText->SetText("0");

	winEditPhis->addCtrl(slidersValText, 110, 25, "acceleration_val");
	winEditPhis->addCtrl(slidersValText, 110, 50, "hp_max_val");
	winEditPhis->addCtrl(slidersValText, 110, 75, "saturation_val");
	winEditPhis->addCtrl(slidersValText, 110, 100, "stamina_val");
	winEditPhis->addCtrl(slidersValText, 110, 125, "fertility_val");
	winEditPhis->addCtrl(slidersValText, 110, 150, "live_time_val");
	winEditPhis->addCtrl(slidersValText, 110, 175, "energy_max_val");
	winEditPhis->addCtrl(slidersValText, 110, 200, "energy_mature_val");
	winEditPhis->addCtrl(slidersValText, 110, 225, "energy_hungry_val");
	winEditPhis->addCtrl(slidersValText, 110, 250, "reproduction_cost_val");
	winEditPhis->addCtrl(slidersValText, 110, 275, "reproduction_time_val");
	winEditPhis->addCtrl(slidersValText, 110, 300, "reproduction_pause_val");

	HTEXTURE editPhisButTex = getButtonTex(200, 20, objsColor, 0.15);
	hgeGUIButton *editPhisBut;
	editPhisBut = new hgeGUIButton(0, 0, 0, 200, 20, editPhisButTex, 0, 0);
	winEditPhis->addCtrl(editPhisBut, 10, 340, "edit_phis_but");

	hgeGUIText *editPhisButText;
	editPhisButText = new hgeGUIText(0, 0, 0, 200, 20, fnt);
	editPhisButText->SetMode(HGETEXT_CENTER);
	editPhisButText->bEnabled = false;
	editPhisButText->SetText("Apply");
	winEditPhis->addCtrl(editPhisButText, 10, 345, "edit_phis_but_t");

	winManager->addWindow(winEditPhis, WIN_EDIT_PHIS);
	////////////////////////////////////////////////////
	///////////////WINDOW EDIT MUTATION/////////////////
	////////////////////////////////////////////////////

	////////////////////////////////////////////////////
	//////////////////WINDOW STATISTIC//////////////////
	////////////////////////////////////////////////////
	//На потом.//
	/////////////

	
	GetWinSlider(winManager, WIN_EDIT_EYE, "angle_s")->SetValue(0);
	GetWinSlider(winManager, WIN_EDIT_EYE, "width_s")->SetValue(10);
	GetWinSlider(winManager, WIN_EDIT_EYE, "height_s")->SetValue(20);
	winManager->Activate(WIN_EDIT_EYE);



	delete indSlider;
	delete slidersValText; 
	delete slidersStaticText;
	delete stateList;

	delete callEyeEditButText;
	delete callEyeEditBut;
	delete eyeSlider;
	delete eyesList;

	delete phisSlider;
	delete editPhisBut;
	delete editPhisButText;
}