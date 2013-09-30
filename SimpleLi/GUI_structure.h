#define GUI_TEXT				0

#define CMD_WIN_S_L				1
#define CMD_WIN_EDIT_DNA		2
#define CMD_WIN_EDIT_PHIS		3
#define CMD_PAUSE				4
#define CMD_CLEAR				5
#define CMD_WIN_EDIT_MUT		6
#define CMD_ZOOM				7
#define CMD_RAND_POP			8

#define WIN_S_L					1
#define WIN_EDIT_DNA			2
#define WIN_EDIT_EYE			3
#define WIN_EDIT_PHIS			4
#define WIN_EDIT_MUT			5
#define WIN_IND_STAT			6
#define WIN_ENV_STAT			7

GeneticCode *genes = new GeneticCode();
Individ_Proto *selectInd = &env.empty;
long long int selectID = 0;

hgeGUIText *slidersValText;
hgeGUIText *slidersStaticText;

RGBColor backCol(0xFF666666);
RGBColor headCol(0xFF999999);
RGBColor colPix(0xFF00DD00);


void resetDNA();
void randDNA();
void randPopulation(int count);
void call_EyeEdit();


class Win_SaveLoad : public GUI_window {
private:
	static Win_SaveLoad *self;
private:
	Win_SaveLoad(DWORD _headCol, DWORD _backCol, DWORD _objsColor):
	   GUI_window(200, 200, "Save/Load", fnt, _headCol, _backCol, _objsColor)
	   {
		   HTEXTURE butSVTex = getButtonTex(80, 15, objsColor, 0.1);
		   hgeGUIButton *butSV;
		   butSV = new hgeGUIButton(0, 0, 0, 80, 15, butSVTex, 0, 0);

		   hgeGUIText *butSVText;
		   butSVText = new hgeGUIText(GUI_TEXT, 0, 0, 80, 15, fnt);
		   butSVText->SetMode(HGETEXT_CENTER);
		   butSVText->SetColor(0xFFFFFFFF);
		   butSVText->bEnabled = false;

		   butSVText->SetText("Save");
		   addCtrl(butSV, 10, 180, "but_save", save_func);
		   addCtrl(butSVText, 10, 185, "but_save_text");
		   butSVText->SetText("Load");
		   addCtrl(butSV, 110, 180, "but_load", load_func);
		   addCtrl(butSVText, 110, 185, "but_load_text");

		   hgeGUIListbox *slotList;
		   slotList = new hgeGUIListbox(0, 0, 0, 200, 150, fnt, 0xFFFFFFFF, 0xFFFFFFFF, objsColor);
		   slotList->AddItem("Slot 1");
		   slotList->AddItem("Slot 2");
		   slotList->AddItem("Slot 3");
		   slotList->AddItem("Slot 4");
		   slotList->AddItem("Slot 5");
		   slotList->AddItem("Slot 6");
		   addCtrl(slotList, 0, 20, "slot_list");
	   };

private:
	static void save_func() {
		switch(GetWinListboxSelect(self, "slot_list")) {
		case 0: env.save("save1.bin"); break;
		case 1: env.save("save2.bin"); break;
		case 2: env.save("save3.bin"); break;
		case 3: env.save("save4.bin"); break;
		case 4: env.save("save5.bin"); break;
		case 5: env.save("save6.bin"); break;
		}
	};

	static void load_func() {
		switch(GetWinListboxSelect(self, "slot_list")) {
		case 0: env.load("save1.bin"); break;
		case 1: env.load("save2.bin"); break;
		case 2: env.load("save3.bin"); break;
		case 3: env.load("save4.bin"); break;
		case 4: env.load("save5.bin"); break;
		case 5: env.load("save6.bin"); break;
		}
	};

	void UpdateFunc() {};

public:
	static Win_SaveLoad *Create(DWORD _headCol, DWORD _backCol, DWORD _objsColor) {
		if (self == NULL) {
			self = new Win_SaveLoad(_headCol, _backCol, _objsColor);
		}
		return self;
	};

	static void Destroy() {
		delete self;
	};
};
Win_SaveLoad *Win_SaveLoad::self = NULL;


class Win_EditEye : public GUI_window {
private:
	static Win_EditEye *self;
	static int selectEye;
private:
	Win_EditEye(DWORD _headCol, DWORD _backCol, DWORD _objsColor): 
	   GUI_window(370, 510, "Edit eye", fnt, _headCol, _backCol, _objsColor) {
		   HTEXTURE *sliderTex = new HTEXTURE(getButtonTex(10, 10, 0xFFFFFFFF, 0.1));

		   hgeQuad cellQ;
		   RGBColor pixCol(1, 1, 0, 0);
		   int areaW = 350, areaH = 350;
		   int countW = 70, countH = 70;
		   int b = 1;
		   int w = areaW/countW - b, h = areaH/countH - b;
		   int pos_x=10, pos_y=25;
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
				   graphic.push_back(cellQ);
			   }
		   }

		   hgeGUISlider *eyeSlider;
		   eyeSlider = new hgeGUISlider(0, 0, 0, 200, 10, *sliderTex, 1, 0, 5, 10);
		   eyeSlider->SetMode(0, 100, HGESLIDER_BAR);
		   eyeSlider->SetValue(0);

		   eyeSlider->SetMode(0, M_PI*2, HGESLIDER_BAR);
		   addCtrl(eyeSlider, 10, 395, "way_angle_s");
		   eyeSlider->SetMode(-M_PI, M_PI, HGESLIDER_BARRELATIVE);
		   addCtrl(eyeSlider, 10, 435, "angle_s");
		   eyeSlider->SetMode(0, 100, HGESLIDER_BAR);
		   addCtrl(eyeSlider, 10, 460, "width_s");
		   addCtrl(eyeSlider, 10, 485, "height_s");

		   slidersStaticText = new hgeGUIText(0, 0, 0, 100, 20, fnt);
		   slidersValText->SetMode(HGETEXT_LEFT);
		   slidersValText->bEnabled = false;

		   slidersValText->SetText("Way:");
		   addCtrl(slidersValText, 10, 385, "way_angle_t");
		   slidersValText->SetText("Eye angle:");
		   addCtrl(slidersValText, 10, 425, "angle_t");
		   slidersValText->SetText("Eye width:");
		   addCtrl(slidersValText, 10, 450, "width_t");
		   slidersValText->SetText("Eye height:");
		   addCtrl(slidersValText, 10, 475, "height_t");

		   slidersValText = new hgeGUIText(0, 0, 0, 100, 20, fnt);
		   slidersValText->SetMode(HGETEXT_RIGHT);
		   slidersValText->bEnabled = false;
		   slidersValText->SetText("0");

		   addCtrl(slidersValText, 110, 385, "way_angle_val");
		   addCtrl(slidersValText, 110, 425, "angle_val");
		   addCtrl(slidersValText, 110, 450, "width_val");
		   addCtrl(slidersValText, 110, 475, "height_val");

		   hgeGUIListbox *eyesList;
		   eyesList = new hgeGUIListbox(0, 0, 0, 30, 115, fnt, 0xFFFFFFFF, 0xFFFFFFFF, objsColor);
		   addCtrl(eyesList, 330, 385, "eyes_list", getEye);

		   HTEXTURE eyeButTex = getButtonTex(50, 50, objsColor, 0.1);
		   hgeGUIButton *eyeBut;
		   eyeBut = new hgeGUIButton(0, 0, 0, 50, 50, eyeButTex, 0, 0);
		   addCtrl(eyeBut, 270, 385, "eye_but_add", addEye);
		   addCtrl(eyeBut, 270, 450, "eye_but_del", delEye);

		   hgeGUIText *eyeButText;
		   eyeButText = new hgeGUIText(0, 0, 0, 50, 50, fnt);
		   eyeButText->SetMode(HGETEXT_CENTER);
		   eyeButText->bEnabled = false;
		   eyeButText->SetText("Add");
		   addCtrl(eyeButText, 270, 388, "eye_but_add_t");
		   eyeButText->SetText("Del");
		   addCtrl(eyeButText, 270, 452, "eye_but_del_t");
	   };

public:
	static void setEye() {
		if(!genes->eyes.empty()) {
			genes->eyes[selectEye].setAngle(GetWinSliderValue(self, "angle_s"));
			genes->eyes[selectEye].setWidth(GetWinSliderValue(self, "width_s"));
			genes->eyes[selectEye].setHeight(GetWinSliderValue(self, "height_s"));
		}
	};

	static void getEye() {
		if(!genes->eyes.empty()) {
			selectEye = GetWinListboxSelect(self, "eyes_list");
			SetWinSliderValue(self, "angle_s", genes->eyes[selectEye].getAngle());
			SetWinSliderValue(self, "width_s", genes->eyes[selectEye].getWidth());
			SetWinSliderValue(self, "height_s", genes->eyes[selectEye].getHeight());
		} else {
			SetWinSliderValue(self, "angle_s", 0);
			SetWinSliderValue(self, "width_s", 0);
			SetWinSliderValue(self, "height_s", 0);
		}
	};

private:
	static void addEye() {
		FOV_Tri e(GetWinSliderValue(self, "angle_s"), 
			GetWinSliderValue(self, "width_s"), 
			GetWinSliderValue(self, "height_s"));

		genes->eyes.push_back(e);
		self->UpdateFunc();
		selectEye = genes->eyes.size()-1;
		SetWinListboxSelect(self, "eyes_list", selectEye);
		GetWinListbox(self, "eyes_list")->SetTopItem(selectEye-4);
	};

	static void delEye() {
		if (!genes->eyes.empty()) {
			genes->eyes.erase(genes->eyes.begin()+selectEye);
			selectEye>0 ? selectEye-- : selectEye = 0;
			SetWinListboxSelect(self, "eyes_list", selectEye);
			getEye();
		}
	};

public:
	static void Reset() {
		selectEye = 0;
		SetWinListboxSelect(self, "eyes_list", 0);
		SetWinSliderValue(self, "angle_s", 0);
		SetWinSliderValue(self, "width_s", 0);
		SetWinSliderValue(self, "height_s", 0);
	}

	void UpdateFunc() {
		SetWinLink(self, "way_angle_s", "way_angle_val");
		SetWinLink(self, "angle_s", "angle_val");
		SetWinLink(self, "width_s", "width_val");
		SetWinLink(self, "height_s", "height_val");

		double wayAngle = GetWinSliderValue(self, "way_angle_s");

		GetWinListbox(self, "eyes_list")->Clear();
		std::vector <hgeQuad> ::iterator p = winManager->getWin(WIN_EDIT_EYE)->graphic.begin();
		while(p != winManager->getWin(WIN_EDIT_EYE)->graphic.end()) {
			setQuadColor(&*p, objsColor);
			p++;
		}

		if(!genes->eyes.empty()) {
			selectEye = GetWinListboxSelect(self, "eyes_list");
			setEye();

			for (int i = 0; i<genes->eyes.size(); i++) {
				GetWinListbox(self, "eyes_list")->AddItem(&Convert(i)[0]);

				if (i != selectEye) {
					std::vector <Vector <int> > tempV = genes->eyes[i].getVectors(wayAngle);
					std::vector <Vector <int> > ::iterator m = tempV.begin();
					while (m != tempV.end()) {
						Vector <int> pix(*m);
						pix.x+=35;
						pix.y+=35;
						if (pix.x >= 0 && pix.y >= 0 && pix.x < 70 && pix.y < 70) 
							setQuadColor(&(winManager->getWin(WIN_EDIT_EYE)->graphic[pix.x + pix.y*70]), 0xFFFF0000);
						m++;
					}
				} 
			}

			std::vector <Vector <int> > tempV = genes->eyes[selectEye].getVectors(wayAngle);
			std::vector <Vector <int> > ::iterator m = tempV.begin();
			while (m != tempV.end()) {
				Vector <int> pix(*m);
				pix.x+=35;
				pix.y+=35;
				if (pix.x >= 0 && pix.y >= 0 && pix.x < 70 && pix.y < 70) 
					setQuadColor(&(winManager->getWin(WIN_EDIT_EYE)->graphic[pix.x + pix.y*70]), 0xFF00FF00);
				m++;
			}
		}

		double sint = sin(wayAngle);
		double cost = cos(wayAngle);
		for (int i = 0; i<20; i++) 
			setQuadColor(&(winManager->getWin(WIN_EDIT_EYE)->graphic[((int)(i*cost))+35 + ((int)(i*sint) + 35)*70]), 0xFFFFFFFF);
	}
public:
	static Win_EditEye *Create(DWORD _headCol, DWORD _backCol, DWORD _objsColor) {
		if (self == NULL) {
			self = new Win_EditEye(_headCol, _backCol, _objsColor);
		}
		return self;
	}

	static void Destroy() {
		delete self;
	}
};
Win_EditEye *Win_EditEye::self = NULL;
int Win_EditEye::selectEye = 0;


class Win_EditPhis : public GUI_window {
private:
	static Win_EditPhis *self;
private:
	Win_EditPhis(DWORD _headCol, DWORD _backCol, DWORD _objsColor): 
	   GUI_window(320, 410, "Edit phis attributes", fnt, _headCol, _backCol, _objsColor) {

		   HTEXTURE *sliderTex = new HTEXTURE(getButtonTex(10, 10, 0xFFFFFFFF, 0.1));

		   hgeGUISlider *phisSlider;
		   phisSlider = new hgeGUISlider(0, 0, 0, 300, 10, *sliderTex, 1, 0, 5, 10);
		   phisSlider->SetValue(0);


		   phisSlider->SetMode(0, 10, HGESLIDER_BAR);
		   addCtrl(phisSlider, 10, 35, "acceleration_s");
		   phisSlider->SetMode(0, 1000, HGESLIDER_BAR);
		   addCtrl(phisSlider, 10, 60, "hp_max_s");
		   phisSlider->SetMode(0, 100, HGESLIDER_BAR);
		   addCtrl(phisSlider, 10, 85, "saturation_s");
		   addCtrl(phisSlider, 10, 110, "stamina_s");
		   addCtrl(phisSlider, 10, 135, "fertility_s");
		   phisSlider->SetMode(0, 1000, HGESLIDER_BAR);
		   addCtrl(phisSlider, 10, 160, "live_time_s");
		   addCtrl(phisSlider, 10, 185, "energy_max_s");
		   addCtrl(phisSlider, 10, 210, "energy_mature_s");
		   addCtrl(phisSlider, 10, 235, "energy_hungry_s");
		   addCtrl(phisSlider, 10, 260, "reproduction_cost_s");
		   addCtrl(phisSlider, 10, 285, "reproduction_time_s");
		   addCtrl(phisSlider, 10, 310, "reproduction_pause_s");

		   slidersStaticText = new hgeGUIText(0, 0, 0, 100, 20, fnt);
		   slidersStaticText->SetMode(HGETEXT_LEFT);
		   slidersStaticText->bEnabled = false;

		   slidersStaticText->SetText("Acceleration:");
		   addCtrl(slidersStaticText, 10, 25, "acceleration_t");
		   slidersStaticText->SetText("Max HP:");
		   addCtrl(slidersStaticText, 10, 50, "hp_max_t");
		   slidersStaticText->SetText("Saturation:");
		   addCtrl(slidersStaticText, 10, 75, "saturation_t");
		   slidersStaticText->SetText("Stamina:");
		   addCtrl(slidersStaticText, 10, 100, "stamina_t");
		   slidersStaticText->SetText("Fertility:");
		   addCtrl(slidersStaticText, 10, 125, "fertility_t");
		   slidersStaticText->SetText("Live time:");
		   addCtrl(slidersStaticText, 10, 150, "live_time_t");
		   slidersStaticText->SetText("Max energy:");
		   addCtrl(slidersStaticText, 10, 175, "energy_max_t");
		   slidersStaticText->SetText("Mature energy:");
		   addCtrl(slidersStaticText, 10, 200, "energy_mature_t");
		   slidersStaticText->SetText("Hungry energy:");
		   addCtrl(slidersStaticText, 10, 225, "energy_hungry_t");
		   slidersStaticText->SetText("Reproduction cost:");
		   addCtrl(slidersStaticText, 10, 250, "reproduction_cost_t");
		   slidersStaticText->SetText("Reproduction time:");
		   addCtrl(slidersStaticText, 10, 275, "reproduction_time_t");
		   slidersStaticText->SetText("Reproduction pause:");
		   addCtrl(slidersStaticText, 10, 300, "reproduction_pause_t");

		   slidersValText = new hgeGUIText(0, 0, 0, 100, 20, fnt);
		   slidersValText->SetMode(HGETEXT_RIGHT);
		   slidersValText->bEnabled = false;
		   slidersValText->SetText("0");

		   addCtrl(slidersValText, 210, 25, "acceleration_val");
		   addCtrl(slidersValText, 210, 50, "hp_max_val");
		   addCtrl(slidersValText, 210, 75, "saturation_val");
		   addCtrl(slidersValText, 210, 100, "stamina_val");
		   addCtrl(slidersValText, 210, 125, "fertility_val");
		   addCtrl(slidersValText, 210, 150, "live_time_val");
		   addCtrl(slidersValText, 210, 175, "energy_max_val");
		   addCtrl(slidersValText, 210, 200, "energy_mature_val");
		   addCtrl(slidersValText, 210, 225, "energy_hungry_val");
		   addCtrl(slidersValText, 210, 250, "reproduction_cost_val");
		   addCtrl(slidersValText, 210, 275, "reproduction_time_val");
		   addCtrl(slidersValText, 210, 300, "reproduction_pause_val");

		   HTEXTURE editPhisButTex = getButtonTex(300, 20, objsColor, 0.1);
		   hgeGUIButton *editPhisBut;
		   editPhisBut = new hgeGUIButton(0, 0, 0, 300, 20, editPhisButTex, 0, 0);

		   hgeGUIText *editPhisButText;
		   editPhisButText = new hgeGUIText(0, 0, 0, 300, 20, fnt);
		   editPhisButText->SetMode(HGETEXT_CENTER);
		   editPhisButText->bEnabled = false;


		   editPhisButText->SetText("Standart");
		   addCtrl(editPhisBut, 10, 355, "standart_phis_but", standart_PhisGenes);
		   addCtrl(editPhisButText, 10, 360, "standart_phis_but_t");

		   editPhisButText->SetText("Apply");
		   addCtrl(editPhisBut, 10, 380, "edit_phis_but", set_PhisGenes);
		   addCtrl(editPhisButText, 10, 385, "edit_phis_but_t");
	   };
public:

	static void get_PhisGenes() {
		SetWinSliderValue(self, "acceleration_s", genes->phis[acceleration]);
		SetWinSliderValue(self, "hp_max_s", genes->phis[hp_max]);
		SetWinSliderValue(self, "saturation_s", genes->phis[saturation]);
		SetWinSliderValue(self, "stamina_s", genes->phis[stamina]);
		SetWinSliderValue(self, "fertility_s", genes->phis[fertility]);
		SetWinSliderValue(self, "live_time_s", genes->phis[live_time]);
		SetWinSliderValue(self, "energy_max_s", genes->phis[energy_max]);
		SetWinSliderValue(self, "energy_mature_s", genes->phis[energy_mature]);
		SetWinSliderValue(self, "energy_hungry_s", genes->phis[energy_hungry]);
		SetWinSliderValue(self, "reproduction_cost_s", genes->phis[reproduction_cost]);
		SetWinSliderValue(self, "reproduction_time_s", genes->phis[reproduction_time]);
		SetWinSliderValue(self, "reproduction_pause_s", genes->phis[reproduction_pause]);
	}

	static void set_PhisGenes() {
		genes->phis[acceleration] = GetWinSliderValue(self, "acceleration_s");
		genes->phis[hp_max] = GetWinSliderValue(self, "hp_max_s");
		genes->phis[saturation] = GetWinSliderValue(self, "saturation_s");
		genes->phis[stamina] = GetWinSliderValue(self, "stamina_s");
		genes->phis[fertility] = GetWinSliderValue(self, "fertility_s");
		genes->phis[live_time] = GetWinSliderValue(self, "live_time_s");
		genes->phis[energy_max] = GetWinSliderValue(self, "energy_max_s");
		genes->phis[energy_mature] = GetWinSliderValue(self, "energy_mature_s");
		genes->phis[energy_hungry] = GetWinSliderValue(self, "energy_hungry_s");
		genes->phis[reproduction_cost] = GetWinSliderValue(self, "reproduction_cost_s");
		genes->phis[reproduction_time] = GetWinSliderValue(self, "reproduction_time_s");
		genes->phis[reproduction_pause] = GetWinSliderValue(self, "reproduction_pause_s");

		if (!env.population.empty()) {
			std::map <long long int, Individ_Proto*> ::iterator p = env.population.begin();
			while (p != env.population.end()) {
				p->second->dna.phis = genes->phis;
				p++;
			}
		}
	}

	static void standart_PhisGenes() {
		genes->phis[acceleration] = 0.3;
		genes->phis[hp_max] = 10; 
		genes->phis[saturation] = 10;
		genes->phis[stamina] = 2;
		genes->phis[fertility] = 4;
		genes->phis[live_time] = 400;
		genes->phis[energy_max] = 1000;
		genes->phis[energy_mature] = 0;
		genes->phis[energy_hungry] = 0;
		genes->phis[reproduction_cost] = 10; 
		genes->phis[reproduction_time] = 10; 
		genes->phis[reproduction_pause] = 100;

		get_PhisGenes();
	};

public:
	void UpdateFunc() {
		SetWinLink(self, "acceleration_s", "acceleration_val");
		SetWinLink(self, "hp_max_s", "hp_max_val");
		SetWinLink(self, "saturation_s", "saturation_val");
		SetWinLink(self, "stamina_s", "stamina_val");
		SetWinLink(self, "fertility_s", "fertility_val");
		SetWinLink(self, "live_time_s", "live_time_val");
		SetWinLink(self, "energy_max_s", "energy_max_val");
		SetWinLink(self, "energy_mature_s", "energy_mature_val");
		SetWinLink(self, "energy_hungry_s", "energy_hungry_val");
		SetWinLink(self, "reproduction_cost_s", "reproduction_cost_val");
		SetWinLink(self, "reproduction_time_s", "reproduction_time_val");
		SetWinLink(self, "reproduction_pause_s", "reproduction_pause_val");
	};

public:
	static Win_EditPhis *Create(DWORD _headCol, DWORD _backCol, DWORD _objsColor) {
		if (self == NULL) {
			self = new Win_EditPhis(_headCol, _backCol, _objsColor);
		}
		return self;
	}

	static void Destroy() {
		delete self;
	};
};
Win_EditPhis *Win_EditPhis::self = NULL;


class Win_EditDNA : public GUI_window {
private:
	static Win_EditDNA *self;
private:
	Win_EditDNA(DWORD _headCol, DWORD _backCol, DWORD _objsColor):
	   GUI_window(380, 305, "DNA", fnt, _headCol, _backCol, _objsColor) {

		   HTEXTURE *sliderTex = new HTEXTURE(getButtonTex(10, 10, 0xFFFFFFFF, 0.1));

		   hgeGUIListbox *list;
		   list = new hgeGUIListbox(0, 0, 0, 100, 100, fnt, 0xFFFFFFFF, 0xFFFFFFFF, objsColor);
		   list->AddItem("HUNGRY");
		   list->AddItem("MATURE");
		   list->AddItem("WAIT");
		   addCtrl(list, 0, 50, "state_list", getDNA);

		   list = new hgeGUIListbox(0, 0, 0, 100, 100, fnt, 0xFFFFFFFF, 0xFFFFFFFF, objsColor);
		   list->AddItem("AUTO");
		   list->AddItem("GETERO");
		   addCtrl(list, 0, 250, "diet_list");


		   hgeGUISlider *indSlider;
		   indSlider = new hgeGUISlider(0, 0, 0, 200, 10, *sliderTex, 1, 0, 5, 10);
		   indSlider->SetValue(0);

		   indSlider->SetMode(0, 10, HGESLIDER_BAR);
		   addCtrl(indSlider, 160, 35, "max_speed_s");
		   indSlider->SetMode(0, M_PI, HGESLIDER_BAR);
		   addCtrl(indSlider, 160, 60, "rand_way_s");
		   indSlider->SetMode(-10, 10, HGESLIDER_BARRELATIVE);
		   addCtrl(indSlider, 160, 85, "libido_s");
		   addCtrl(indSlider, 160, 110, "partner_s");
		   addCtrl(indSlider, 160, 135, "cohesion_partner_s");
		   addCtrl(indSlider, 160, 160, "separation_partner_s");
		   addCtrl(indSlider, 160, 185, "alignment_partner_s");
		   addCtrl(indSlider, 160, 210, "enemy_s");
		   addCtrl(indSlider, 160, 235, "cohesion_enemy_s");
		   addCtrl(indSlider, 160, 260, "separation_enemy_s");
		   addCtrl(indSlider, 160, 285, "alignment_enemy_s");

		   slidersStaticText = new hgeGUIText(0, 0, 0, 100, 20, fnt);
		   slidersStaticText->SetMode(HGETEXT_LEFT);
		   slidersStaticText->bEnabled = false;

		   slidersStaticText->SetText("Max speed:");
		   addCtrl(slidersStaticText, 160, 25, "max_speed_t");
		   slidersStaticText->SetText("Rand way:");
		   addCtrl(slidersStaticText, 160, 50, "rand_way_t");
		   slidersStaticText->SetText("libido:");
		   addCtrl(slidersStaticText, 160, 75, "libido_t");
		   slidersStaticText->SetText("Partners effect:");
		   addCtrl(slidersStaticText, 160, 100, "partner_t");
		   slidersStaticText->SetText("Partners cohesion:");
		   addCtrl(slidersStaticText, 160, 125, "cohesion_partner_t");
		   slidersStaticText->SetText("Partners separation:");
		   addCtrl(slidersStaticText, 160, 150, "separation_partner_t");
		   slidersStaticText->SetText("Partners alignment:");
		   addCtrl(slidersStaticText, 160, 175, "alignment_partner_t");
		   slidersStaticText->SetText("Enemies effect:");
		   addCtrl(slidersStaticText, 160, 200, "enemy_t");
		   slidersStaticText->SetText("Enemies cohesion:");
		   addCtrl(slidersStaticText, 160, 225, "cohesion_enemy_t");
		   slidersStaticText->SetText("Enemies separation:");
		   addCtrl(slidersStaticText, 160, 250, "separation_enemy_t");
		   slidersStaticText->SetText("Enemies alignment:");
		   addCtrl(slidersStaticText, 160, 275, "alignment_enemy_t");

		   slidersValText = new hgeGUIText(0, 0, 0, 100, 20, fnt);
		   slidersValText->SetMode(HGETEXT_RIGHT);
		   slidersValText->bEnabled = false;
		   slidersValText->SetText("0");

		   addCtrl(slidersValText, 260, 25, "max_speed_val");
		   addCtrl(slidersValText, 260, 50, "rand_way_val");
		   addCtrl(slidersValText, 260, 75, "libido_val");
		   addCtrl(slidersValText, 260, 100, "partner_val");
		   addCtrl(slidersValText, 260, 125, "cohesion_partner_val");
		   addCtrl(slidersValText, 260, 150, "separation_partner_val");
		   addCtrl(slidersValText, 260, 175, "alignment_partner_val");
		   addCtrl(slidersValText, 260, 200, "enemy_val");
		   addCtrl(slidersValText, 260, 225, "cohesion_enemy_val");
		   addCtrl(slidersValText, 260, 250, "separation_enemy_val");
		   addCtrl(slidersValText, 260, 275, "alignment_enemy_val");

		   HTEXTURE dnaEditButtonsTex = getButtonTex(140, 20, objsColor, 0.1);
		   hgeGUIButton *dnaEditButtons;
		   dnaEditButtons = new hgeGUIButton(0, 0, 0, 140, 20, dnaEditButtonsTex, 0, 0);


		   hgeGUIText *callEyeEditButText;
		   callEyeEditButText = new hgeGUIText(0, 0, 0, 140, 20, fnt);
		   callEyeEditButText->SetMode(HGETEXT_CENTER);
		   callEyeEditButText->bEnabled = false;

		   
		   addCtrl(dnaEditButtons, 10, 175, "rand_dna_but", randDNA);
		   callEyeEditButText->SetText("Rand DNA");
		   addCtrl(callEyeEditButText, 10, 180, "rand_dna_but_t");

		   addCtrl(dnaEditButtons, 10, 200, "new_dna_but", resetDNA);
		   callEyeEditButText->SetText("New DNA");
		   addCtrl(callEyeEditButText, 10, 205, "new_dna_but_t");

		   addCtrl(dnaEditButtons, 10, 225, "edit_eyes_but", call_EyeEdit);
		   callEyeEditButText->SetText("Edit eyes");
		   addCtrl(callEyeEditButText, 10, 230, "edit_eyes_but_t");
	   };

public:
	static void call_EyeEdit() {
		winManager->Activate(WIN_EDIT_EYE);
		winManager->setFocus(WIN_EDIT_EYE);
		Win_EditEye::getEye();
	}

	static void resetDNA() {
		delete genes;
		genes = new GeneticCode();
		Win_EditPhis::standart_PhisGenes();
		Win_EditDNA::getDNA();
	}

	static void randDNA() {
		delete genes;
		genes = new GeneticCode();
		genes->randomize();
		Win_EditPhis::standart_PhisGenes();
		getDNA();
		Win_EditEye::getEye();
	}

	static void getDNA() {
		SetWinListboxSelect(self, "diet_list", genes->diet);
		int select_state = GetWinListboxSelect(self, "state_list");

		SetWinSliderValue(self, "max_speed_s", genes->soc[select_state][max_speed]);
		SetWinSliderValue(self, "rand_way_s", genes->soc[select_state][rand_way]);
		SetWinSliderValue(self, "libido_s", genes->soc[select_state][libido]);
		SetWinSliderValue(self, "partner_s", genes->soc[select_state][partner]);
		SetWinSliderValue(self, "cohesion_partner_s", genes->soc[select_state][cohesion_partner]);
		SetWinSliderValue(self, "separation_partner_s", genes->soc[select_state][separation_partner]);
		SetWinSliderValue(self, "alignment_partner_s", genes->soc[select_state][alignment_partner]);
		SetWinSliderValue(self, "enemy_s", genes->soc[select_state][enemy]);
		SetWinSliderValue(self, "cohesion_enemy_s", genes->soc[select_state][cohesion_enemy]);
		SetWinSliderValue(self, "separation_enemy_s", genes->soc[select_state][separation_enemy]);
		SetWinSliderValue(self, "alignment_enemy_s", genes->soc[select_state][alignment_enemy]);
	}

	static void setDNA() {
		genes->diet = GetWinListboxSelect(self, "diet_list");
		int select_state = GetWinListboxSelect(self, "state_list");
		genes->soc[select_state][max_speed] = GetWinSliderValue(self, "max_speed_s");
		genes->soc[select_state][rand_way] = GetWinSliderValue(self, "rand_way_s");
		genes->soc[select_state][libido] = GetWinSliderValue(self, "libido_s");
		genes->soc[select_state][partner] = GetWinSliderValue(self, "partner_s");
		genes->soc[select_state][cohesion_partner] = GetWinSliderValue(self, "cohesion_partner_s");
		genes->soc[select_state][separation_partner] = GetWinSliderValue(self, "separation_partner_s");
		genes->soc[select_state][alignment_partner] = GetWinSliderValue(self, "alignment_partner_s");
		genes->soc[select_state][enemy] = GetWinSliderValue(self, "enemy_s");
		genes->soc[select_state][cohesion_enemy] = GetWinSliderValue(self, "cohesion_enemy_s");
		genes->soc[select_state][separation_enemy] = GetWinSliderValue(self, "separation_enemy_s");
		genes->soc[select_state][alignment_enemy] = GetWinSliderValue(self, "alignment_enemy_s");
	}
private:
	void UpdateFunc() {
		setDNA();
		SetWinLink(self, "max_speed_s", "max_speed_val");
		SetWinLink(self, "rand_way_s", "rand_way_val");
		SetWinLink(self, "libido_s", "libido_val");
		SetWinLink(self, "partner_s", "partner_val");
		SetWinLink(self, "cohesion_partner_s", "cohesion_partner_val");
		SetWinLink(self, "separation_partner_s", "separation_partner_val");
		SetWinLink(self, "alignment_partner_s", "alignment_partner_val");
		SetWinLink(self, "enemy_s", "enemy_val");
		SetWinLink(self, "cohesion_enemy_s", "cohesion_enemy_val");
		SetWinLink(self, "separation_enemy_s", "separation_enemy_val");
		SetWinLink(self, "alignment_enemy_s", "alignment_enemy_val");
	};

public:

	static Win_EditDNA *Create(DWORD _headCol, DWORD _backCol, DWORD _objsColor) {
		if (self == NULL) {
			self = new Win_EditDNA(_headCol, _backCol, _objsColor);
		}
		return self;
	};

	static void Destroy() {
		delete self;
	};
};
Win_EditDNA *Win_EditDNA::self = NULL;


class Win_EditMut : public GUI_window {
private:
	static Win_EditMut *self;
private:
	Win_EditMut(DWORD _headCol, DWORD _backCol, DWORD _objsColor):
	   GUI_window(250, 185, "Edit mutation", fnt, _headCol, _backCol, _objsColor) {

		   HTEXTURE *sliderTex = new HTEXTURE(getButtonTex(10, 10, 0xFFFFFFFF, 0.1));

		   hgeGUISlider *mutSlider;
		   mutSlider = new hgeGUISlider(0, 0, 0, 230, 10, *sliderTex, 1, 0, 5, 10);
		   mutSlider->SetValue(0);

		   mutSlider->SetMode(0, 1, HGESLIDER_BAR);
		   addCtrl(mutSlider, 10, 35, "mutation_maxDelta_s");
		   mutSlider->SetMode(0, end_of_soc*end_of_status, HGESLIDER_BAR);
		   addCtrl(mutSlider, 10, 60, "mutation_mutGenCount_s");
		   mutSlider->SetMode(0, 1, HGESLIDER_BAR);
		   addCtrl(mutSlider, 10, 85, "mutation_eyeAddChance_s");
		   addCtrl(mutSlider, 10, 110, "mutation_eyeMutationChance_s");
		   addCtrl(mutSlider, 10, 135, "mutation_radEyeMutationChance_s");

		   delete slidersStaticText;
		   slidersStaticText = new hgeGUIText(0, 0, 0, 100, 20, fnt);
		   slidersStaticText->SetMode(HGETEXT_LEFT);
		   slidersStaticText->bEnabled = false;

		   slidersStaticText->SetText("Deviation degree:");
		   addCtrl(slidersStaticText, 10, 25, "mutation_maxDelta_t");
		   slidersStaticText->SetText("Count of mutation genes:");
		   addCtrl(slidersStaticText, 10, 50, "mutation_mutGenCount_t");
		   slidersStaticText->SetText("Chance to add eye:");
		   addCtrl(slidersStaticText, 10, 75, "mutation_eyeAddChance_t");
		   slidersStaticText->SetText("Eye mutation chance:");
		   addCtrl(slidersStaticText, 10, 100, "mutation_eyeMutationChance_t");
		   slidersStaticText->SetText("Radial eye mutation:");
		   addCtrl(slidersStaticText, 10, 125, "mutation_radEyeMutationChance_t");

		   delete slidersValText;
		   slidersValText = new hgeGUIText(0, 0, 0, 100, 20, fnt);
		   slidersValText->SetMode(HGETEXT_RIGHT);
		   slidersValText->bEnabled = false;
		   slidersValText->SetText("0");

		   addCtrl(slidersValText, 140, 25, "mutation_maxDelta_val");
		   addCtrl(slidersValText, 140, 50, "mutation_mutGenCount_val");
		   addCtrl(slidersValText, 140, 75, "mutation_eyeAddChance_val");
		   addCtrl(slidersValText, 140, 100, "mutation_eyeMutationChance_val");
		   addCtrl(slidersValText, 140, 125, "mutation_radEyeMutationChance_val");

		   HTEXTURE editMutButTex = getButtonTex(230, 20, objsColor, 0.1);
		   hgeGUIButton *editMutBut;
		   editMutBut = new hgeGUIButton(0, 0, 0, 230, 20, editMutButTex, 0, 0);

		   hgeGUIText *editMutButText;
		   editMutButText = new hgeGUIText(0, 0, 0, 230, 20, fnt);
		   editMutButText->SetMode(HGETEXT_CENTER);
		   editMutButText->bEnabled = false;

		   editMutButText->SetText("Apply");
		   addCtrl(editMutBut, 10, 155, "edit_mut_but", set_Mut);
		   addCtrl(editMutButText, 10, 160, "edit_mut_but_t");

	   }

public:
	static void set_Mut() {
		env.setMutation(
			GetWinSliderValue(self, "mutation_maxDelta_s"),
			GetWinSliderValue(self, "mutation_mutGenCount_s"),
			GetWinSliderValue(self, "mutation_eyeAddChance_s"),
			GetWinSliderValue(self, "mutation_eyeMutationChance_s"),
			GetWinSliderValue(self, "mutation_radEyeMutationChance_s")
			);
	}
private:
	void UpdateFunc() {
		SetWinLink(self, "mutation_maxDelta_s", "mutation_maxDelta_val");
		SetWinLink(self, "mutation_eyeAddChance_s", "mutation_eyeAddChance_val");
		SetWinLink(self, "mutation_eyeMutationChance_s", "mutation_eyeMutationChance_val");
		SetWinLink(self, "mutation_radEyeMutationChance_s", "mutation_radEyeMutationChance_val");
		SetWinSliderValue(self, "mutation_mutGenCount_s", 
			(int)GetWinSliderValue(self, "mutation_mutGenCount_s"));
		SetWinLink(self, "mutation_mutGenCount_s", "mutation_mutGenCount_val");
	};

public:
	static Win_EditMut *Create(DWORD _headCol, DWORD _backCol, DWORD _objsColor) {
		if (self == NULL) {
			self = new Win_EditMut(_headCol, _backCol, _objsColor);
		}
		return self;
	}

	static void Destroy() {
		delete self;
	};
};
Win_EditMut *Win_EditMut::self = NULL;


class Win_IndStat : public GUI_window {
private:
	static Win_IndStat *self;
private:
	Win_IndStat(DWORD _headCol, DWORD _backCol, DWORD _objsColor):
	   GUI_window(200, 120, "Individ Status", fnt, headCol, backCol, objsColor) {

		   hgeGUIText *val;
		   val = new hgeGUIText(0, 0, 0, 90, 10, fnt);
		   val->SetMode(HGETEXT_RIGHT);
		   val->bEnabled = false;
		   val->SetText("0");

		   hgeGUIText *valText;
		   valText = new hgeGUIText(0, 0, 0, 90, 10, fnt);
		   valText->SetMode(HGETEXT_LEFT);
		   valText->bEnabled = false;
		   //Сделать окошко, где бы отображалось нынешнее положение на поле, центрированное на особи
		   //там же добавить направление вектора движения

		   valText->SetText("ID:");
		   addCtrl(valText, 10, 25, "id_t");
		   addCtrl(val, 100, 25, "id_val");
		   valText->SetText("Diet:");
		   addCtrl(valText, 10, 35, "diet_t");
		   addCtrl(val, 100, 35, "diet_val");
		   valText->SetText("Gender:");
		   addCtrl(valText, 10, 45, "gender_t");
		   addCtrl(val, 100, 45, "gender_val");
		   valText->SetText("Status:");
		   addCtrl(valText, 10, 55, "state_t");
		   addCtrl(val, 100, 55, "state_val");
		   valText->SetText("HP:");
		   addCtrl(valText, 10, 65, "hp_t");
		   addCtrl(val, 100, 65, "hp_val");
		   valText->SetText("Energy");
		   addCtrl(valText, 10, 75, "energy_t");
		   addCtrl(val, 100, 75, "energy_val");
		   valText->SetText("Live timer:");
		   addCtrl(valText, 10, 85, "live_timer_t");
		   addCtrl(val, 100, 85, "live_timer_val");
		   valText->SetText("Reproduction timer:");
		   addCtrl(valText, 10, 95, "reproduction_timer_t");
		   addCtrl(val, 100, 95, "reproduction_timer_val");
		   valText->SetText("Speed:");
		   addCtrl(valText, 10, 105, "speed_t");
		   addCtrl(val, 100, 105, "speed_val");

	   };
public:
	void UpdateFunc() {
		if (env.population.find(selectID) == env.population.end()) {
			selectInd = &env.empty;
			selectID = 0;
		}
		SetWinValLink(self, selectInd->ID, "id_val");
		switch (selectInd->dna.diet) {
		case 0: SetWinText(self, "diet_val", "AUTO"); break;
		case 1: SetWinText(self, "diet_val", "GETERO"); break;
		}
		switch (selectInd->gender) {
		case 0: SetWinText(self, "gender_val", "MALE"); break;
		case 1: SetWinText(self, "gender_val", "FEMALE"); break;
		case 2: SetWinText(self, "gender_val", "NEUTER"); break;
		}
		switch (selectInd->state) {
		case 0: SetWinText(self, "state_val", "HUNGRY"); break;
		case 1: SetWinText(self, "state_val", "MATURE"); break;
		case 2: SetWinText(self, "state_val", "WAIT"); break;
		case 4: SetWinText(self, "state_val", "REPRODUCTIN"); break;
		case 5: SetWinText(self, "state_val", "STOP"); break;
		case 6: SetWinText(self, "state_val", "EAT"); break;
		}
		SetWinValLink(self, selectInd->hp, "hp_val");
		SetWinValLink(self, selectInd->energy, "energy_val");
		SetWinValLink(self, selectInd->live_timer, "live_timer_val");
		SetWinValLink(self, selectInd->reproduction_timer, "reproduction_timer_val");
		SetWinValLink(self, selectInd->speed, "speed_val");
	};

public:
	static void upd() {
		self->UpdateFunc();
	}
	static Win_IndStat *Create(DWORD _headCol, DWORD _backCol, DWORD _objsColor) {
		if (self == NULL) {
			self = new Win_IndStat(_headCol, _backCol, _objsColor);
		}
		return self;
	}
	static void Destroy() {
		delete self;
	}
};
Win_IndStat *Win_IndStat::self = NULL;


class Win_EnvStat : public GUI_window {
private:
	static Win_EnvStat *self;
private:
	Win_EnvStat(DWORD _headCol, DWORD _backCol, DWORD _objsColor):
	   GUI_window(200, 60, "Environment Statistic", fnt, headCol, backCol, objsColor) {


		   hgeGUIText *val = new hgeGUIText(0, 0, 0, 90, 10, fnt);
		   val->SetMode(HGETEXT_RIGHT);
		   val->bEnabled = false;
		   val->SetText("0");

		   hgeGUIText *valText = new hgeGUIText(0, 0, 0, 90, 10, fnt);
		   valText->SetMode(HGETEXT_LEFT);
		   valText->bEnabled = false;

		   valText->SetText("FPS:");
		   addCtrl(valText, 10, 25, "fps_t");
		   addCtrl(val, 100, 25, "fps_val");
		   valText->SetText("Population:");
		   addCtrl(valText, 10, 35, "population_t");
		   addCtrl(val, 100, 35, "population_val");
		   valText->SetText("Step:");
		   addCtrl(valText, 10, 45, "step_t");
		   addCtrl(val, 100, 45, "step_val");

	   }
public:
	void UpdateFunc() {
		SetWinValLink(self, hge->Timer_GetFPS(), "fps_val");
		SetWinValLink(self, env.population.size(), "population_val");
		SetWinValLink(self, env.stepCount, "step_val");
	}

public:
	static void upd() {
		self->UpdateFunc();
	}
	static Win_EnvStat *Create(DWORD _headCol, DWORD _backCol, DWORD _objsColor) {
		if (self == NULL) {
			self = new Win_EnvStat(_headCol, _backCol, _objsColor);
		}
		return self;
	}

	static void Destroy() {
		delete self;
	}
};
Win_EnvStat *Win_EnvStat::self = NULL;

//переписать//
void randPopulation(int count) {
	env.clear();
	genes->diet = AUTO;
	GeneticCode temp = *genes;
	for (int i=0; i<count; i++) {
		genes->randomize();
		env.addIndivid(
			new Individ_Auto(Vector <int> (func::randi(0, env.W()-2), func::randi(0, env.H()-2)), *genes)
			);
	}
	(*genes) = temp;
}

void CreateWinManager() {
	winManager->addWindow(Win_SaveLoad::Create(headCol, backCol, objsColor), WIN_S_L);
	winManager->addWindow(Win_EditDNA::Create(headCol, backCol, objsColor), WIN_EDIT_DNA);
	winManager->addWindow(Win_EditEye::Create(headCol, backCol, objsColor), WIN_EDIT_EYE);
	winManager->addWindow(Win_EditPhis::Create(headCol, backCol, objsColor), WIN_EDIT_PHIS);
	Win_EditPhis::standart_PhisGenes();
	winManager->addWindow(Win_EditMut::Create(headCol, backCol, objsColor), WIN_EDIT_MUT);
	winManager->addWindow(Win_IndStat::Create(headCol, backCol, objsColor), WIN_IND_STAT, 600, 150);
	winManager->addWindow(Win_EnvStat::Create(headCol, backCol, objsColor), WIN_ENV_STAT, 600, 0);
	winManager->Activate(WIN_ENV_STAT);
}

void CreateMainGUI() {
	hgeGUIButton *button;
	hgeGUIText	 *text;

	HTEXTURE butTex = getButtonTex(180, 20, objsColor, 0.1);

	button=new hgeGUIButton(CMD_WIN_S_L, 610, 350, 180, 20, butTex, 0, 0);
	button->SetMode(false);
	mainGUI->AddCtrl(button);

	text = new hgeGUIText(GUI_TEXT, 610, 350, 180, 30, fnt);
	text->SetMode(HGETEXT_CENTER);
	text->SetColor(0xFFFFFFFF);
	text->SetText("Save/Load");
	text->bEnabled = false;
	mainGUI->AddCtrl(text);

	button=new hgeGUIButton(CMD_WIN_EDIT_MUT, 610, 400, 180, 20, butTex, 0, 0);
	button->SetMode(false);
	mainGUI->AddCtrl(button);

	text = new hgeGUIText(GUI_TEXT, 610, 400, 180, 30, fnt);
	text->SetMode(HGETEXT_CENTER);
	text->SetColor(0xFFFFFFFF);
	text->SetText("Set mutation");
	text->bEnabled = false;
	mainGUI->AddCtrl(text);

	button=new hgeGUIButton(CMD_WIN_EDIT_DNA, 610, 430, 180, 20, butTex, 0, 0);
	button->SetMode(false);
	mainGUI->AddCtrl(button);

	text = new hgeGUIText(GUI_TEXT, 610, 430, 180, 30, fnt);
	text->SetMode(HGETEXT_CENTER);
	text->SetColor(0xFFFFFFFF);
	text->SetText("Edit DNA");
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

	button=new hgeGUIButton(CMD_CLEAR, 610, 490, 180, 20, butTex, 0, 0);
	button->SetMode(false);
	mainGUI->AddCtrl(button);

	text = new hgeGUIText(GUI_TEXT, 610, 490, 180, 30, fnt);
	text->SetMode(HGETEXT_CENTER);
	text->SetColor(0xFFFFFFFF);
	text->SetText("Clear");
	text->bEnabled = false;
	mainGUI->AddCtrl(text);

	button=new hgeGUIButton(CMD_RAND_POP, 610, 520, 180, 20, butTex, 0, 0);
	button->SetMode(false);
	mainGUI->AddCtrl(button);

	text = new hgeGUIText(GUI_TEXT, 610, 520, 180, 30, fnt);
	text->SetMode(HGETEXT_CENTER);
	text->SetColor(0xFFFFFFFF);
	text->SetText("Rand Population");
	text->bEnabled = false;
	mainGUI->AddCtrl(text);

	HTEXTURE Tex = getButtonTex(10, 10, 0xFFFFFFFF, 0.1);

	text = new hgeGUIText(GUI_TEXT, 610, 285, 180, 30, fnt);
	text->SetMode(HGETEXT_CENTER);
	text->SetColor(0xFFFFFFFF);
	text->SetText("Zoom");
	text->bEnabled = false;
	mainGUI->AddCtrl(text);

	hgeGUISlider *slid;
	slid = new hgeGUISlider(CMD_ZOOM, 610, 300, 180, 10, Tex, 1, 0, 5, 10);
	slid->SetMode(1, env.H()/10, HGESLIDER_BAR);
	slid->SetValue(1);
	mainGUI->AddCtrl(slid);
}

void CheckButtons() {
	//Запихать в окно
	if (!winManager->checkHit(state.mp.x, state.mp.y)) {
		if (hgeButtonGetState(mainGUI, CMD_WIN_S_L)) {
			winManager->Activate(WIN_S_L);
			winManager->setFocus(WIN_S_L);
		}
		if (hgeButtonGetState(mainGUI, CMD_WIN_EDIT_MUT)) {
			winManager->Activate(WIN_EDIT_MUT);
			winManager->setFocus(WIN_EDIT_MUT);
		}
		if (hgeButtonGetState(mainGUI, CMD_WIN_EDIT_DNA)) {
			winManager->Activate(WIN_EDIT_DNA);
			winManager->setFocus(WIN_EDIT_DNA);
		}
		if (hgeButtonGetState(mainGUI, CMD_WIN_EDIT_PHIS)) {
			winManager->Activate(WIN_EDIT_PHIS);
			winManager->setFocus(WIN_EDIT_PHIS);

			Win_EditPhis::get_PhisGenes();
		}
		if (hgeButtonGetState(mainGUI, CMD_CLEAR)) {
			env.clear();
		}
		if (hgeButtonGetState(mainGUI, CMD_RAND_POP)) {
			randPopulation(sqrt((double)(env.H()*env.W())));
		}
		if (hgeButtonGetState(mainGUI, CMD_PAUSE)) {
			state.play = false;
		} else state.play = true;

		if (zoom != hgeSliderGetValue(mainGUI, CMD_ZOOM)) {
			zoom = hgeSliderGetValue(mainGUI, CMD_ZOOM);
			display->setZoom(zoom);
		}
	}
}

void CheckKeys() {
	int col, row;
	Win_IndStat::upd();
	Win_EnvStat::upd();

	if (hge->Input_GetKeyState(HGEK_UP)) {
		display->setPos(display->getVisibleX(), display->getVisibleY()-1);
	}
	if (hge->Input_GetKeyState(HGEK_DOWN)) {
		display->setPos(display->getVisibleX(), display->getVisibleY()+1);
	}
	if (hge->Input_GetKeyState(HGEK_LEFT)) {
		display->setPos(display->getVisibleX()-1, display->getVisibleY());
	}
	if (hge->Input_GetKeyState(HGEK_RIGHT)) {
		display->setPos(display->getVisibleX()+1, display->getVisibleY());
	}
	if (hge->Input_KeyDown(HGEK_SPACE)) {
		hgeButtonSetState(mainGUI, CMD_PAUSE, !hgeButtonGetState(mainGUI, CMD_PAUSE));
		state.play = !state.play;
	}

	//Мыша
	if (hge->Input_GetKeyState(HGEK_LBUTTON) && 
		display->getMousePos(state.mp.x, state.mp.y, &col, &row) &&
		!winManager->checkHit(state.mp.x, state.mp.y) &&
		env.field(col, row)->ID != 0) 
	{
		*genes = env.field(col, row)->dna;
		selectInd = env.field(col, row);
		selectID = selectInd->ID;
		Win_EditDNA::getDNA();

		Win_EditEye::Reset();
		Win_EditEye::getEye();

		winManager->getWin(WIN_EDIT_DNA)->Render();
		winManager->getWin(WIN_EDIT_EYE)->Render();
		winManager->Activate(WIN_IND_STAT);
		winManager->getWin(WIN_IND_STAT)->Render();
	}

	if (hge->Input_GetKeyState(HGEK_RBUTTON) && 
		display->getMousePos(state.mp.x, state.mp.y, &col, &row) &&
		!winManager->checkHit(state.mp.x, state.mp.y)) {
		env.addIndivid(CreateIndivid(Vector <int> (col, row), *genes));
	}
}


class Win_example : public GUI_window {
private:
	static Win_example *self;
private:
	Win_example(DWORD _headCol, DWORD _backCol, DWORD _objsColor):
	   GUI_window(0, 0, "", fnt, _headCol, _backCol, _objsColor) {
	   }
private:
	void UpdateFunc() {};

public:
	static Win_example *Create(DWORD _headCol, DWORD _backCol, DWORD _objsColor) {
		if (self == NULL) {
			self = new Win_example(_headCol, _backCol, _objsColor);
		}
		return self;
	}

	static void Destroy() {
		delete self;
	};
};
Win_example *Win_example::self = NULL;