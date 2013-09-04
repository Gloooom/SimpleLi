char buf[10];

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

void call_EyeEdit() {
	winManager->Activate(WIN_EDIT_EYE);
	winManager->setFocus(WIN_EDIT_EYE);
}

void update_EyeEdit() {
	SetWinText(winManager, WIN_EDIT_EYE, "angle_val", 
		itoa((int)((GetWinSliderValue(winManager, WIN_EDIT_EYE, "angle_s")/100)*360), buf, 10));
	SetWinText(winManager, WIN_EDIT_EYE, "width_val", 
		itoa((int)(GetWinSliderValue(winManager, WIN_EDIT_EYE, "width_s")), buf, 10));
	SetWinText(winManager, WIN_EDIT_EYE, "height_val", 
		itoa((int)(GetWinSliderValue(winManager, WIN_EDIT_EYE, "height_s")), buf, 10));


	//заготовка для метода look индивида.
	//какая-то странаня херня с поворотом шаблона. Возможно дело в округлении точек при повороте. А возможно и нет.
	//Добавить красивые затухающие окошки об каких-то событиях в центре экрана. Больше кавая, да!
	std::vector <hgeQuad> ::iterator p = winManager->getWin(WIN_EDIT_EYE)->graphic.begin();
	while(p != winManager->getWin(WIN_EDIT_EYE)->graphic.end()) {
		setQuadColor(&*p, objsColor);
		p++;
	}

	Point <int> pos(35, 35);
	std::vector <Vector <double> > mem;

	FOV_Tri eee(0, (GetWinSliderValue(winManager, WIN_EDIT_EYE, "height_s")), 
					(GetWinSliderValue(winManager, WIN_EDIT_EYE, "width_s")));
	double wayAngle = 0; 
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

	HTEXTURE *sliderTex = new HTEXTURE(getButtonTex(10, 10, 0xFFFFFFFF, 0.1));

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

	winEditEye->setUpdateFunc(update_EyeEdit);

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
	//winManager->Activate(WIN_EDIT_EYE);



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