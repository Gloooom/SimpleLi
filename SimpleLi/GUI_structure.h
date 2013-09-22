#define GUI_TEXT				0

#define CMD_WIN_S_L				1
#define CMD_WIN_EDIT_DNA			2
#define CMD_WIN_EDIT_PHIS		3
#define CMD_PAUSE				4
#define CMD_CLEAR				5
#define CMD_WIN_EDIT_MUT		6
#define CMD_ZOOM				7

#define WIN_S_L					1
#define WIN_EDIT_DNA				2
#define WIN_EDIT_EYE			3
#define WIN_EDIT_PHIS			4
#define WIN_EDIT_MUT			5


bool *isDNASelect = new bool(false);
#define SetLink(winID, sliderID, textID) SetWinText(winManager, winID, textID, Convert(GetWinSliderValue(winManager, winID, sliderID)).c_str())
GeneticCode *genes = new GeneticCode();

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

void state_list_upd() {
	int select_state = GetWinListboxSelect(winManager, WIN_EDIT_DNA, "state_list");
	SetWinSliderValue(winManager, WIN_EDIT_DNA, "max_speed_s", genes->soc[select_state][max_speed]);
	SetWinSliderValue(winManager, WIN_EDIT_DNA, "rand_way_s", genes->soc[select_state][rand_way]);
	SetWinSliderValue(winManager, WIN_EDIT_DNA, "libido_s", genes->soc[select_state][libido]);
	SetWinSliderValue(winManager, WIN_EDIT_DNA, "partner_s", genes->soc[select_state][partner]);
	SetWinSliderValue(winManager, WIN_EDIT_DNA, "cohesion_partner_s", genes->soc[select_state][cohesion_partner]);
	SetWinSliderValue(winManager, WIN_EDIT_DNA, "separation_partner_s", genes->soc[select_state][separation_partner]);
	SetWinSliderValue(winManager, WIN_EDIT_DNA, "alignment_partner_s", genes->soc[select_state][alignment_partner]);
	SetWinSliderValue(winManager, WIN_EDIT_DNA, "enemy_s", genes->soc[select_state][enemy]);
	SetWinSliderValue(winManager, WIN_EDIT_DNA, "cohesion_enemy_s", genes->soc[select_state][cohesion_enemy]);
	SetWinSliderValue(winManager, WIN_EDIT_DNA, "separation_enemy_s", genes->soc[select_state][separation_enemy]);
	SetWinSliderValue(winManager, WIN_EDIT_DNA, "alignment_enemy_s", genes->soc[select_state][alignment_enemy]);
}

void resetDNA() {
	genes = new GeneticCode();
	state_list_upd();
}

void update_IndEdit() {
	int select_state = GetWinListboxSelect(winManager, WIN_EDIT_DNA, "state_list");
	genes->soc[select_state][max_speed] = GetWinSliderValue(winManager, WIN_EDIT_DNA, "max_speed_s");
	genes->soc[select_state][rand_way] = GetWinSliderValue(winManager, WIN_EDIT_DNA, "rand_way_s");
	genes->soc[select_state][libido] = GetWinSliderValue(winManager, WIN_EDIT_DNA, "libido_s");
	genes->soc[select_state][partner] = GetWinSliderValue(winManager, WIN_EDIT_DNA, "partner_s");
	genes->soc[select_state][cohesion_partner] = GetWinSliderValue(winManager, WIN_EDIT_DNA, "cohesion_partner_s");
	genes->soc[select_state][separation_partner] = GetWinSliderValue(winManager, WIN_EDIT_DNA, "separation_partner_s");
	genes->soc[select_state][alignment_partner] = GetWinSliderValue(winManager, WIN_EDIT_DNA, "alignment_partner_s");
	genes->soc[select_state][enemy] = GetWinSliderValue(winManager, WIN_EDIT_DNA, "enemy_s");
	genes->soc[select_state][cohesion_enemy] = GetWinSliderValue(winManager, WIN_EDIT_DNA, "cohesion_enemy_s");
	genes->soc[select_state][separation_enemy] = GetWinSliderValue(winManager, WIN_EDIT_DNA, "separation_enemy_s");
	genes->soc[select_state][alignment_enemy] = GetWinSliderValue(winManager, WIN_EDIT_DNA, "alignment_enemy_s");

	SetLink(WIN_EDIT_DNA, "max_speed_s", "max_speed_val");
	SetLink(WIN_EDIT_DNA, "rand_way_s", "rand_way_val");
	SetLink(WIN_EDIT_DNA, "libido_s", "libido_val");
	SetLink(WIN_EDIT_DNA, "partner_s", "partner_val");
	SetLink(WIN_EDIT_DNA, "cohesion_partner_s", "cohesion_partner_val");
	SetLink(WIN_EDIT_DNA, "separation_partner_s", "separation_partner_val");
	SetLink(WIN_EDIT_DNA, "alignment_partner_s", "alignment_partner_val");
	SetLink(WIN_EDIT_DNA, "enemy_s", "enemy_val");
	SetLink(WIN_EDIT_DNA, "cohesion_enemy_s", "cohesion_enemy_val");
	SetLink(WIN_EDIT_DNA, "separation_enemy_s", "separation_enemy_val");
	SetLink(WIN_EDIT_DNA, "alignment_enemy_s", "alignment_enemy_val");
}

void randDNA() {
	for (int i=0; i<end_of_status; i++) {
		genes->soc[i][max_speed] = func::randf(0, 10);
		genes->soc[i][libido] = func::randf(-10, 10);
		genes->soc[i][rand_way] = func::randf(0, M_PI);
		genes->soc[i][partner] = func::randf(-10, 10);
		genes->soc[i][cohesion_partner] = func::randf(-10, 10);
		genes->soc[i][separation_partner] = func::randf(-10, 10);
		genes->soc[i][alignment_partner] = func::randf(-10, 10);
		genes->soc[i][enemy] = func::randf(-10, 10);
		genes->soc[i][cohesion_enemy] = func::randf(-10, 10);
		genes->soc[i][separation_enemy] = func::randf(-10, 10);
		genes->soc[i][alignment_enemy] = func::randf(-10, 10);
	}
	state_list_upd();
}

int selectEye;
void update_EyeEdit();

void setEye() {
	if(!genes->eyes.empty()) {
		genes->eyes[selectEye].setAngle(GetWinSliderValue(winManager, WIN_EDIT_EYE, "angle_s"));
		genes->eyes[selectEye].setWidth(GetWinSliderValue(winManager, WIN_EDIT_EYE, "width_s"));
		genes->eyes[selectEye].setHeight(GetWinSliderValue(winManager, WIN_EDIT_EYE, "height_s"));
	}
}

void getEye() {
	if(!genes->eyes.empty()) {
		selectEye = GetWinListboxSelect(winManager, WIN_EDIT_EYE, "eyes_list");
		SetWinSliderValue(winManager, WIN_EDIT_EYE, "angle_s", genes->eyes[selectEye].getAngle());
		SetWinSliderValue(winManager, WIN_EDIT_EYE, "width_s", genes->eyes[selectEye].getWidth());
		SetWinSliderValue(winManager, WIN_EDIT_EYE, "height_s", genes->eyes[selectEye].getHeight());
	}
}

void addEye() {
	FOV_Tri e(GetWinSliderValue(winManager, WIN_EDIT_EYE, "angle_s"), 
		GetWinSliderValue(winManager, WIN_EDIT_EYE, "width_s"), 
		GetWinSliderValue(winManager, WIN_EDIT_EYE, "height_s"));

	genes->eyes.push_back(e);
	update_EyeEdit();
	selectEye = genes->eyes.size()-1;
	SetWinListboxSelect(winManager, WIN_EDIT_EYE, "eyes_list", selectEye);
	GetWinListbox(winManager, WIN_EDIT_EYE, "eyes_list")->SetTopItem(selectEye-4);
}

void delEye() {
	if (!genes->eyes.empty()) {
		genes->eyes.erase(genes->eyes.begin()+selectEye);
		selectEye>0 ? selectEye-- : selectEye = 0;
		SetWinListboxSelect(winManager, WIN_EDIT_EYE, "eyes_list", selectEye);
		getEye();
	}
}

void update_EyeEdit() {
	SetLink(WIN_EDIT_EYE, "way_angle_s", "way_angle_val");
	SetLink(WIN_EDIT_EYE, "angle_s", "angle_val");
	SetLink(WIN_EDIT_EYE, "width_s", "width_val");
	SetLink(WIN_EDIT_EYE, "height_s", "height_val");

	double wayAngle = GetWinSliderValue(winManager, WIN_EDIT_EYE, "way_angle_s");

	GetWinListbox(winManager, WIN_EDIT_EYE, "eyes_list")->Clear();
	std::vector <hgeQuad> ::iterator p = winManager->getWin(WIN_EDIT_EYE)->graphic.begin();
	while(p != winManager->getWin(WIN_EDIT_EYE)->graphic.end()) {
		setQuadColor(&*p, objsColor);
		p++;
	}

	if(!genes->eyes.empty()) {
		selectEye = GetWinListboxSelect(winManager, WIN_EDIT_EYE, "eyes_list");
		setEye();
		
		for (int i = 0; i<genes->eyes.size(); i++) {
			GetWinListbox(winManager, WIN_EDIT_EYE, "eyes_list")->AddItem(&Convert(i)[0]);

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

void call_EyeEdit() {
	winManager->Activate(WIN_EDIT_EYE);
	winManager->setFocus(WIN_EDIT_EYE);
	getEye();
}

void update_PhisEdit() {
	SetLink(WIN_EDIT_PHIS, "acceleration_s", "acceleration_val");
	SetLink(WIN_EDIT_PHIS, "hp_max_s", "hp_max_val");
	SetLink(WIN_EDIT_PHIS, "saturation_s", "saturation_val");
	SetLink(WIN_EDIT_PHIS, "stamina_s", "stamina_val");
	SetLink(WIN_EDIT_PHIS, "fertility_s", "fertility_val");
	SetLink(WIN_EDIT_PHIS, "live_time_s", "live_time_val");
	SetLink(WIN_EDIT_PHIS, "energy_max_s", "energy_max_val");
	SetLink(WIN_EDIT_PHIS, "energy_mature_s", "energy_mature_val");
	SetLink(WIN_EDIT_PHIS, "energy_hungry_s", "energy_hungry_val");
	SetLink(WIN_EDIT_PHIS, "reproduction_cost_s", "reproduction_cost_val");
	SetLink(WIN_EDIT_PHIS, "reproduction_time_s", "reproduction_time_val");
	SetLink(WIN_EDIT_PHIS, "reproduction_pause_s", "reproduction_pause_val");
}

void get_PhisGenes() {
	SetWinSliderValue(winManager, WIN_EDIT_PHIS, "acceleration_s", genes->phis[acceleration]);
	SetWinSliderValue(winManager, WIN_EDIT_PHIS, "hp_max_s", genes->phis[hp_max]);
	SetWinSliderValue(winManager, WIN_EDIT_PHIS, "saturation_s", genes->phis[saturation]);
	SetWinSliderValue(winManager, WIN_EDIT_PHIS, "stamina_s", genes->phis[stamina]);
	SetWinSliderValue(winManager, WIN_EDIT_PHIS, "fertility_s", genes->phis[fertility]);
	SetWinSliderValue(winManager, WIN_EDIT_PHIS, "live_time_s", genes->phis[live_time]);
	SetWinSliderValue(winManager, WIN_EDIT_PHIS, "energy_max_s", genes->phis[energy_max]);
	SetWinSliderValue(winManager, WIN_EDIT_PHIS, "energy_mature_s", genes->phis[energy_mature]);
	SetWinSliderValue(winManager, WIN_EDIT_PHIS, "energy_hungry_s", genes->phis[energy_hungry]);
	SetWinSliderValue(winManager, WIN_EDIT_PHIS, "reproduction_cost_s", genes->phis[reproduction_cost]);
	SetWinSliderValue(winManager, WIN_EDIT_PHIS, "reproduction_time_s", genes->phis[reproduction_time]);
	SetWinSliderValue(winManager, WIN_EDIT_PHIS, "reproduction_pause_s", genes->phis[reproduction_pause]);
}

void set_PhisGenes() {
	genes->phis[acceleration] = GetWinSliderValue(winManager, WIN_EDIT_PHIS, "acceleration_s");
	genes->phis[hp_max] = GetWinSliderValue(winManager, WIN_EDIT_PHIS, "hp_max_s");
	genes->phis[saturation] = GetWinSliderValue(winManager, WIN_EDIT_PHIS, "saturation_s");
	genes->phis[stamina] = GetWinSliderValue(winManager, WIN_EDIT_PHIS, "stamina_s");
	genes->phis[fertility] = GetWinSliderValue(winManager, WIN_EDIT_PHIS, "fertility_s");
	genes->phis[live_time] = GetWinSliderValue(winManager, WIN_EDIT_PHIS, "live_time_s");
	genes->phis[energy_max] = GetWinSliderValue(winManager, WIN_EDIT_PHIS, "energy_max_s");
	genes->phis[energy_mature] = GetWinSliderValue(winManager, WIN_EDIT_PHIS, "energy_mature_s");
	genes->phis[energy_hungry] = GetWinSliderValue(winManager, WIN_EDIT_PHIS, "energy_hungry_s");
	genes->phis[reproduction_cost] = GetWinSliderValue(winManager, WIN_EDIT_PHIS, "reproduction_cost_s");
	genes->phis[reproduction_time] = GetWinSliderValue(winManager, WIN_EDIT_PHIS, "reproduction_time_s");
	genes->phis[reproduction_pause] = GetWinSliderValue(winManager, WIN_EDIT_PHIS, "reproduction_pause_s");

	if (!env.population.empty()) {
		std::map <long long int, Individ> ::iterator p = env.population.begin();
		while (p != env.population.end()) {
			p->second.dna.phis = genes->phis;
			p++;
		}
	}
}

void standart_PhisGenes() {
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
}

void set_Mut() {
	env.setMutation(
		GetWinSliderValue(winManager, WIN_EDIT_MUT, "mutation_maxDelta_s"),
		GetWinSliderValue(winManager, WIN_EDIT_MUT, "mutation_mutGenCount_s"),
		GetWinSliderValue(winManager, WIN_EDIT_MUT, "mutation_eyeAddChance_s"),
		GetWinSliderValue(winManager, WIN_EDIT_MUT, "mutation_eyeMutationChance_s"),
		GetWinSliderValue(winManager, WIN_EDIT_MUT, "mutation_radEyeMutationChance_s")
		);
}

void update_MutEdit() {
	SetLink(WIN_EDIT_MUT, "mutation_maxDelta_s", "mutation_maxDelta_val");
	SetLink(WIN_EDIT_MUT, "mutation_eyeAddChance_s", "mutation_eyeAddChance_val");
	SetLink(WIN_EDIT_MUT, "mutation_eyeMutationChance_s", "mutation_eyeMutationChance_val");
	SetLink(WIN_EDIT_MUT, "mutation_radEyeMutationChance_s", "mutation_radEyeMutationChance_val");
	SetWinSliderValue(winManager, WIN_EDIT_MUT, "mutation_mutGenCount_s", 
		(int)GetWinSliderValue(winManager, WIN_EDIT_MUT, "mutation_mutGenCount_s"));
	SetLink(WIN_EDIT_MUT, "mutation_mutGenCount_s", "mutation_mutGenCount_val");
}

void CreateWinManager() {
	RGBColor backCol(0xFF666666);
	RGBColor headCol(0xFF999999);
	RGBColor colPix(0xFF00DD00);

	HTEXTURE *sliderTex = new HTEXTURE(getButtonTex(10, 10, 0xFFFFFFFF, 0.1));

	////////////////////////////////////////////////////
	//////////////////////WINDOWS///////////////////////
	////////////////////////////////////////////////////
	////////////////WINDOW SAVE AND LOAD////////////////
	////////////////////////////////////////////////////
	GUI_window *winSaveLoad;
	winSaveLoad = new GUI_window(200, 200, "Save/Load", fnt, headCol, backCol, objsColor);
	
	HTEXTURE butSVTex = getButtonTex(80, 15, objsColor, 0.1);
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
	////////////////WINDOW EDIT DNA//////////////////
	////////////////////////////////////////////////////
	GUI_window *winAddIndivid;
	winAddIndivid = new GUI_window(380, 305, "DNA", fnt, headCol, backCol, objsColor);
	winAddIndivid->setUpdateFunc(update_IndEdit);


	hgeGUIListbox *stateList;
	stateList = new hgeGUIListbox(0, 0, 0, 100, 100, fnt, 0xFFFFFFFF, 0xFFFFFFFF, objsColor);
	stateList->AddItem("HUNGRY");
	stateList->AddItem("MATURE");
	stateList->AddItem("WAIT");
	winAddIndivid->addCtrl(stateList, 0, 50, "state_list", state_list_upd);

	hgeGUISlider *indSlider;
	indSlider = new hgeGUISlider(0, 0, 0, 200, 10, *sliderTex, 1, 0, 5, 10);
	indSlider->SetValue(0);
	
	indSlider->SetMode(0, 10, HGESLIDER_BAR);
	winAddIndivid->addCtrl(indSlider, 160, 35, "max_speed_s");
	indSlider->SetMode(0, M_PI, HGESLIDER_BAR);
	winAddIndivid->addCtrl(indSlider, 160, 60, "rand_way_s");
	indSlider->SetMode(-10, 10, HGESLIDER_BARRELATIVE);
	winAddIndivid->addCtrl(indSlider, 160, 85, "libido_s");
	winAddIndivid->addCtrl(indSlider, 160, 110, "partner_s");
	winAddIndivid->addCtrl(indSlider, 160, 135, "cohesion_partner_s");
	winAddIndivid->addCtrl(indSlider, 160, 160, "separation_partner_s");
	winAddIndivid->addCtrl(indSlider, 160, 185, "alignment_partner_s");
	winAddIndivid->addCtrl(indSlider, 160, 210, "enemy_s");
	winAddIndivid->addCtrl(indSlider, 160, 235, "cohesion_enemy_s");
	winAddIndivid->addCtrl(indSlider, 160, 260, "separation_enemy_s");
	winAddIndivid->addCtrl(indSlider, 160, 285, "alignment_enemy_s");

	hgeGUIText *slidersStaticText;
	slidersStaticText = new hgeGUIText(0, 0, 0, 100, 20, fnt);
	slidersStaticText->SetMode(HGETEXT_LEFT);
	slidersStaticText->bEnabled = false;

	slidersStaticText->SetText("Max speed:");
	winAddIndivid->addCtrl(slidersStaticText, 160, 25, "max_speed_t");
	slidersStaticText->SetText("Rand way:");
	winAddIndivid->addCtrl(slidersStaticText, 160, 50, "rand_way_t");
	slidersStaticText->SetText("libido:");
	winAddIndivid->addCtrl(slidersStaticText, 160, 75, "libido_t");
	slidersStaticText->SetText("Partners effect:");
	winAddIndivid->addCtrl(slidersStaticText, 160, 100, "partner_t");
	slidersStaticText->SetText("Partners cohesion:");
	winAddIndivid->addCtrl(slidersStaticText, 160, 125, "cohesion_partner_t");
	slidersStaticText->SetText("Partners separation:");
	winAddIndivid->addCtrl(slidersStaticText, 160, 150, "separation_partner_t");
	slidersStaticText->SetText("Partners alignment:");
	winAddIndivid->addCtrl(slidersStaticText, 160, 175, "alignment_partner_t");
	slidersStaticText->SetText("Enemies effect:");
	winAddIndivid->addCtrl(slidersStaticText, 160, 200, "enemy_t");
	slidersStaticText->SetText("Enemies cohesion:");
	winAddIndivid->addCtrl(slidersStaticText, 160, 225, "cohesion_enemy_t");
	slidersStaticText->SetText("Enemies separation:");
	winAddIndivid->addCtrl(slidersStaticText, 160, 250, "separation_enemy_t");
	slidersStaticText->SetText("Enemies alignment:");
	winAddIndivid->addCtrl(slidersStaticText, 160, 275, "alignment_enemy_t");

	hgeGUIText *slidersValText;
	slidersValText = new hgeGUIText(0, 0, 0, 100, 20, fnt);
	slidersValText->SetMode(HGETEXT_RIGHT);
	slidersValText->bEnabled = false;
	slidersValText->SetText("0");

	winAddIndivid->addCtrl(slidersValText, 260, 25, "max_speed_val");
	winAddIndivid->addCtrl(slidersValText, 260, 50, "rand_way_val");
	winAddIndivid->addCtrl(slidersValText, 260, 75, "libido_val");
	winAddIndivid->addCtrl(slidersValText, 260, 100, "partner_val");
	winAddIndivid->addCtrl(slidersValText, 260, 125, "cohesion_partner_val");
	winAddIndivid->addCtrl(slidersValText, 260, 150, "separation_partner_val");
	winAddIndivid->addCtrl(slidersValText, 260, 175, "alignment_partner_val");
	winAddIndivid->addCtrl(slidersValText, 260, 200, "enemy_val");
	winAddIndivid->addCtrl(slidersValText, 260, 225, "cohesion_enemy_val");
	winAddIndivid->addCtrl(slidersValText, 260, 250, "separation_enemy_val");
	winAddIndivid->addCtrl(slidersValText, 260, 275, "alignment_enemy_val");

	HTEXTURE indEditButtonsTex = getButtonTex(140, 20, objsColor, 0.1);
	hgeGUIButton *indEditButtons;
	indEditButtons = new hgeGUIButton(0, 0, 0, 140, 20, indEditButtonsTex, 0, 0);
	
	winAddIndivid->addCtrl(indEditButtons, 10, 175, "rand_dna_but", randDNA);
	winAddIndivid->addCtrl(indEditButtons, 10, 200, "new_dna_but", resetDNA);
	winAddIndivid->addCtrl(indEditButtons, 10, 225, "edit_eyes_but", call_EyeEdit);

	hgeGUIText *callEyeEditButText;
	callEyeEditButText = new hgeGUIText(0, 0, 0, 140, 20, fnt);
	callEyeEditButText->SetMode(HGETEXT_CENTER);
	callEyeEditButText->bEnabled = false;

	callEyeEditButText->SetText("Rand DNA");
	winAddIndivid->addCtrl(callEyeEditButText, 10, 180, "rand_dna_but_t");
	callEyeEditButText->SetText("New DNA");
	winAddIndivid->addCtrl(callEyeEditButText, 10, 205, "new_dna_but_t");
	callEyeEditButText->SetText("Edit eyes");
	winAddIndivid->addCtrl(callEyeEditButText, 10, 230, "edit_eyes_but_t");

	winManager->addWindow(winAddIndivid, WIN_EDIT_DNA);
	state_list_upd();
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
			winEditEye->graphic.push_back(cellQ);
		}
	}

	hgeGUISlider *eyeSlider;
	eyeSlider = new hgeGUISlider(0, 0, 0, 200, 10, *sliderTex, 1, 0, 5, 10);
	eyeSlider->SetMode(0, 100, HGESLIDER_BAR);
	eyeSlider->SetValue(0);
	
	eyeSlider->SetMode(0, M_PI*2, HGESLIDER_BAR);
	winEditEye->addCtrl(eyeSlider, 10, 395, "way_angle_s");
	eyeSlider->SetMode(-M_PI, M_PI, HGESLIDER_BARRELATIVE);
	winEditEye->addCtrl(eyeSlider, 10, 435, "angle_s");
	eyeSlider->SetMode(0, 100, HGESLIDER_BAR);
	winEditEye->addCtrl(eyeSlider, 10, 460, "width_s");
	winEditEye->addCtrl(eyeSlider, 10, 485, "height_s");

	slidersStaticText = new hgeGUIText(0, 0, 0, 100, 20, fnt);
	slidersValText->SetMode(HGETEXT_LEFT);
	slidersValText->bEnabled = false;
	
	slidersValText->SetText("Way:");
	winEditEye->addCtrl(slidersValText, 10, 385, "way_angle_t");
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

	winEditEye->addCtrl(slidersValText, 110, 385, "way_angle_val");
	winEditEye->addCtrl(slidersValText, 110, 425, "angle_val");
	winEditEye->addCtrl(slidersValText, 110, 450, "width_val");
	winEditEye->addCtrl(slidersValText, 110, 475, "height_val");

	hgeGUIListbox *eyesList;
	eyesList = new hgeGUIListbox(0, 0, 0, 30, 115, fnt, 0xFFFFFFFF, 0xFFFFFFFF, objsColor);
	winEditEye->addCtrl(eyesList, 330, 385, "eyes_list", getEye);

	HTEXTURE eyeButTex = getButtonTex(50, 50, objsColor, 0.1);
	hgeGUIButton *eyeBut;
	eyeBut = new hgeGUIButton(0, 0, 0, 50, 50, eyeButTex, 0, 0);
	winEditEye->addCtrl(eyeBut, 270, 385, "eye_but_add", addEye);
	winEditEye->addCtrl(eyeBut, 270, 450, "eye_but_del", delEye);
	
	hgeGUIText *eyeButText;
	eyeButText = new hgeGUIText(0, 0, 0, 50, 50, fnt);
	eyeButText->SetMode(HGETEXT_CENTER);
	eyeButText->bEnabled = false;
	eyeButText->SetText("Add");
	winEditEye->addCtrl(eyeButText, 270, 388, "eye_but_add_t");
	eyeButText->SetText("Del");
	winEditEye->addCtrl(eyeButText, 270, 452, "eye_but_del_t");

	winEditEye->setUpdateFunc(update_EyeEdit);

	winManager->addWindow(winEditEye, WIN_EDIT_EYE);
	////////////////////////////////////////////////////
	////////////////WINDOW EDIT PHIS////////////////////
	////////////////////////////////////////////////////
	GUI_window *winEditPhis;
	winEditPhis = new GUI_window(220, 410, "Edit phis attributes", fnt, headCol, backCol, objsColor);

	hgeGUISlider *phisSlider;
	phisSlider = new hgeGUISlider(0, 0, 0, 200, 10, *sliderTex, 1, 0, 5, 10);
	phisSlider->SetValue(0);
	
	
	phisSlider->SetMode(0, 10, HGESLIDER_BAR);
	winEditPhis->addCtrl(phisSlider, 10, 35, "acceleration_s");
	phisSlider->SetMode(0, 1000, HGESLIDER_BAR);
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

	HTEXTURE editPhisButTex = getButtonTex(200, 20, objsColor, 0.1);
	hgeGUIButton *editPhisBut;
	editPhisBut = new hgeGUIButton(0, 0, 0, 200, 20, editPhisButTex, 0, 0);
	
	hgeGUIText *editPhisButText;
	editPhisButText = new hgeGUIText(0, 0, 0, 200, 20, fnt);
	editPhisButText->SetMode(HGETEXT_CENTER);
	editPhisButText->bEnabled = false;

	
	editPhisButText->SetText("Standart");
	winEditPhis->addCtrl(editPhisBut, 10, 355, "standart_phis_but", standart_PhisGenes);
	winEditPhis->addCtrl(editPhisButText, 10, 360, "standart_phis_but_t");

	editPhisButText->SetText("Apply");
	winEditPhis->addCtrl(editPhisBut, 10, 380, "edit_phis_but", set_PhisGenes);
	winEditPhis->addCtrl(editPhisButText, 10, 385, "edit_phis_but_t");

	winEditPhis->setUpdateFunc(update_PhisEdit);

	winManager->addWindow(winEditPhis, WIN_EDIT_PHIS);
	standart_PhisGenes();
	////////////////////////////////////////////////////
	///////////////WINDOW EDIT MUTATION/////////////////
	////////////////////////////////////////////////////
	GUI_window *winEditMut;
	winEditMut = new GUI_window(250, 185, "Edit mutation", fnt, headCol, backCol, objsColor);

	hgeGUISlider *mutSlider;
	mutSlider = new hgeGUISlider(0, 0, 0, 230, 10, *sliderTex, 1, 0, 5, 10);
	mutSlider->SetValue(0);
	
	
	mutSlider->SetMode(0, 1, HGESLIDER_BAR);
	winEditMut->addCtrl(mutSlider, 10, 35, "mutation_maxDelta_s");
	mutSlider->SetMode(0, end_of_soc*end_of_status, HGESLIDER_BAR);
	winEditMut->addCtrl(mutSlider, 10, 60, "mutation_mutGenCount_s");
	mutSlider->SetMode(0, 1, HGESLIDER_BAR);
	winEditMut->addCtrl(mutSlider, 10, 85, "mutation_eyeAddChance_s");
	winEditMut->addCtrl(mutSlider, 10, 110, "mutation_eyeMutationChance_s");
	winEditMut->addCtrl(mutSlider, 10, 135, "mutation_radEyeMutationChance_s");

	slidersStaticText = new hgeGUIText(0, 0, 0, 100, 20, fnt);
	slidersStaticText->SetMode(HGETEXT_LEFT);
	slidersStaticText->bEnabled = false;

	slidersStaticText->SetText("Deviation degree:");
	winEditMut->addCtrl(slidersStaticText, 10, 25, "mutation_maxDelta_t");
	slidersStaticText->SetText("Count of mutation genes:");
	winEditMut->addCtrl(slidersStaticText, 10, 50, "mutation_mutGenCount_t");
	slidersStaticText->SetText("Chance to add eye:");
	winEditMut->addCtrl(slidersStaticText, 10, 75, "mutation_eyeAddChance_t");
	slidersStaticText->SetText("Eye mutation chance:");
	winEditMut->addCtrl(slidersStaticText, 10, 100, "mutation_eyeMutationChance_t");
	slidersStaticText->SetText("Radial eye mutation:");
	winEditMut->addCtrl(slidersStaticText, 10, 125, "mutation_radEyeMutationChance_t");

	slidersValText = new hgeGUIText(0, 0, 0, 100, 20, fnt);
	slidersValText->SetMode(HGETEXT_RIGHT);
	slidersValText->bEnabled = false;
	slidersValText->SetText("0");

	winEditMut->addCtrl(slidersValText, 140, 25, "mutation_maxDelta_val");
	winEditMut->addCtrl(slidersValText, 140, 50, "mutation_mutGenCount_val");
	winEditMut->addCtrl(slidersValText, 140, 75, "mutation_eyeAddChance_val");
	winEditMut->addCtrl(slidersValText, 140, 100, "mutation_eyeMutationChance_val");
	winEditMut->addCtrl(slidersValText, 140, 125, "mutation_radEyeMutationChance_val");

	HTEXTURE editMutButTex = getButtonTex(230, 20, objsColor, 0.1);
	hgeGUIButton *editMutBut;
	editMutBut = new hgeGUIButton(0, 0, 0, 230, 20, editMutButTex, 0, 0);
	
	hgeGUIText *editMutButText;
	editMutButText = new hgeGUIText(0, 0, 0, 230, 20, fnt);
	editMutButText->SetMode(HGETEXT_CENTER);
	editMutButText->bEnabled = false;

	editMutButText->SetText("Apply");
	winEditMut->addCtrl(editMutBut, 10, 155, "edit_mut_but", set_Mut);
	winEditMut->addCtrl(editMutButText, 10, 160, "edit_mut_but_t");

	winEditMut->setUpdateFunc(update_MutEdit);

	winManager->addWindow(winEditMut, WIN_EDIT_MUT);

	////////////////////////////////////////////////////
	//////////////////WINDOW STATISTIC//////////////////
	////////////////////////////////////////////////////
	//На потом.//
	/////////////

	delete indSlider;
	delete slidersValText; 
	delete slidersStaticText;
	delete stateList;

	delete callEyeEditButText;
	delete eyeSlider;
	delete eyesList;

	delete phisSlider;
	delete editPhisBut;
	delete editPhisButText;
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
	
	HTEXTURE Tex = getButtonTex(10, 10, 0xFFFFFFFF, 0.1);

	text = new hgeGUIText(GUI_TEXT, 610, 285, 180, 30, fnt);
	text->SetMode(HGETEXT_CENTER);
	text->SetColor(0xFFFFFFFF);
	text->SetText("Zoom");
	text->bEnabled = false;
	mainGUI->AddCtrl(text);

	hgeGUISlider *slid;
	slid = new hgeGUISlider(CMD_ZOOM, 610, 300, 180, 10, Tex, 1, 0, 5, 10);
	slid->SetMode(0, env.H()/10, HGESLIDER_BAR);
	slid->SetValue(1);
	mainGUI->AddCtrl(slid);
}

void CheckButtons() {
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

		get_PhisGenes();
	}
	if (hgeButtonGetState(mainGUI, CMD_CLEAR)) {
		env.clear();
	}
	if (hgeButtonGetState(mainGUI, CMD_PAUSE)) {
		state.play = false;
	} else state.play = true;

	if (zoom != hgeSliderGetValue(mainGUI, CMD_ZOOM)) {
		zoom = hgeSliderGetValue(mainGUI, CMD_ZOOM);
		display->setZoom(zoom);
	}
}

void CheckKeys() {
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
	}

	int col, row;
	if (hge->Input_GetKeyState(HGEK_LBUTTON) && 
		display->getMousePos(state.mp.x, state.mp.y, &col, &row) &&
		!winManager->checkHit(state.mp.x, state.mp.y) &&
		env.field(col, row)->ID != 0) 
	{
		*genes = env.field(col, row)->dna;
		state_list_upd();
		getEye();
		update_EyeEdit();
		
		winManager->Activate(WIN_EDIT_DNA);
		winManager->setFocus(WIN_EDIT_DNA);
		winManager->getWin(WIN_EDIT_DNA)->Render();
		winManager->getWin(WIN_EDIT_EYE)->Render();
	}
	if (hge->Input_GetKeyState(HGEK_RBUTTON) && 
		display->getMousePos(state.mp.x, state.mp.y, &col, &row) &&
		!winManager->checkHit(state.mp.x, state.mp.y)) 
	{
		genes->diet = AUTO;
		env.addIndivid(Individ(Point <int>(col, row), *genes));
	}
}