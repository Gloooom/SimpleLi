#include "Gui_win_manager.h"

GUI_win_manager::GUI_win_manager() {
	diaTex = Pixel(0x00000000).getTex();
	gui = new hgeGUI();
};

void GUI_win_manager::addWindow(GUI_window *win, int ID, float startPosX, float startPosY) {
	win->ID = ID;
	windows[ID] = win;

	gui->AddCtrl(new hgeGUIButton(ID, win->x, win->y, win->w, win->h, diaTex, 0, 0));
	((hgeGUIButton*)gui->GetCtrl(ID))->SetMode(false);
	gui->EnableCtrl(ID, false);

	setWinPos(ID, startPosX, startPosY);
};

GUI_window* GUI_win_manager::getFocusWin() {
	return windows.at(activeWindows.front());
};

GUI_window* GUI_win_manager::getWin(int ID) {
	return windows.at(ID);
};

int GUI_win_manager::getFocus() {
	if (!activeWindows.empty())
		return activeWindows.front();
	else return 0;
};

void GUI_win_manager::setFocus(int ID) {
	std::deque <int> ::iterator futureFrontID = std::find(activeWindows.begin(), activeWindows.end(), ID);
	if (futureFrontID != activeWindows.end()) {
		windows.at(*activeWindows.begin())->setAColor(0x99);

		activeWindows.erase(futureFrontID);
		activeWindows.push_front(ID);

		windows.at(ID)->setAColor(0xFF);
	}
};

void GUI_win_manager::Update(float dt, float mx, float my) {
	if (!activeWindows.empty()) {
		GUI_window *top = getFocusWin();
		
		top->Update(dt, mx, my);
		
		gui->Update(dt);
		gui->MoveCtrl(getFocus(), windows[getFocus()]->x, windows[getFocus()]->y);
		

		if (!(mx > top->x && mx < (top->x + top->w) &&
			my > top->y && my < (top->y + top->h))) { 
				std::deque <int> ::reverse_iterator but = activeWindows.rbegin();
				while (but != activeWindows.rend()) {
					if(hgeButtonGetState(gui, *but)) {
						setFocus(*but);
						hgeButtonSetState(gui, *but, false);
						break;
					} else but++;
				}
		}

		std::deque <int> ::iterator p = activeWindows.begin();
		while (p != activeWindows.end()) {
			if (!windows.at(*p)->visible) {
				gui->EnableCtrl(*p, false);
				p = activeWindows.erase(p);
				windows.at(*activeWindows.begin())->setAColor(0xFF);
			} else p++;
		}
	}
};

void GUI_win_manager::Render() {
	std::deque <int> ::reverse_iterator p = activeWindows.rbegin();
	while (p != activeWindows.rend()) {
		windows.at(*p)->Render();
		p++;
	}
};

void GUI_win_manager::Activate(int ID) {
	std::deque <int> ::iterator futureActive = std::find(activeWindows.begin(), activeWindows.end(), ID);
	if (futureActive == activeWindows.end()) {
		windows.at(ID)->visible = true;
		if (activeWindows.empty()) 
			windows.at(ID)->setAColor(0xFF); 
		else 
			windows.at(ID)->setAColor(0x99);
		activeWindows.push_back(ID);
		gui->EnableCtrl(ID, true);
	} 
};

void GUI_win_manager::Deactivate(int ID) {
	activeWindows.erase(std::find(activeWindows.begin(), activeWindows.end(), ID));
	windows.at(ID)->visible = false;
	gui->EnableCtrl(ID, false);
}

void GUI_win_manager::setWinPos(int ID, float _x, float _y) {
	windows.at(ID)->setPos(_x, _y); 
	gui->MoveCtrl(ID, _x, _y); 
};