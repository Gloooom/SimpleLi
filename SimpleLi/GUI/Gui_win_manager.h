#include "Gui_types.h"
#include "Gui_window.h"
#include "Gui_color.h"

#define GetWinButton(manager, winid, ctrlname) ((hgeGUIButton*)(manager->getWin(winid)->getCtrl(ctrlname)))
#define GetWinSlider(manager, winid, ctrlname) ((hgeGUISlider*)(manager->getWin(winid)->getCtrl(ctrlname)))
#define GetWinListbox(manager, winid, ctrlname) ((hgeGUIListbox*)(manager->getWin(winid)->getCtrl(ctrlname)))

#define GetWinButtonState(manager, winid, ctrlname) (((hgeGUIButton*)(manager->getWin(winid)->getCtrl(ctrlname)))->GetState())
#define GetWinSliderValue(manager, winid, ctrlname) (((hgeGUISlider*)(manager->getWin(winid)->getCtrl(ctrlname)))->GetValue())
#define GetWinListboxSelect(manager, winid, ctrlname) (((hgeGUIListbox*)(manager->getWin(winid)->getCtrl(ctrlname)))->GetSelectedItem())

#define SetWinButtonState(manager, winid, ctrlname, state) (((hgeGUIButton*)(manager->getWin(winid)->getCtrl(ctrlname)))->SetState(state))
#define SetWinSliderValue(manager, winid, ctrlname, val) (((hgeGUISlider*)(manager->getWin(winid)->getCtrl(ctrlname)))->SetValue(val))
#define SetWinListboxSelect(manager, winid, ctrlname, select) (((hgeGUIListbox*)(manager->getWin(winid)->getCtrl(ctrlname)))->SetSelectedItem(select))

class GUI_win_manager {
private:
	std::map <int, GUI_window*> windows;
	std::deque <int> activeWindows;
	hgeGUI *gui;
	Pixel diaPix;
public:
	GUI_win_manager():
	diaPix(0x00000000)
	{
		gui = new hgeGUI();
	};
	void addWindow(GUI_window *win, int ID) {
		win->ID = ID;
		windows[ID] = win;

		gui->AddCtrl(new hgeGUIButton(ID, win->x, win->y, win->x+win->w, win->y+win->h, diaPix.getTex(), 0, 0));
		((hgeGUIButton*)gui->GetCtrl(ID))->SetMode(false);
	};
	GUI_window *getFocusWin() {
		return windows.at(activeWindows.front());
	};
	GUI_window *getWin(int ID) {
		return windows.at(ID);
	};

	int getFocusWinID() {
		if (!activeWindows.empty())
			return activeWindows.front();
		else return 0;
	};
	void setFocus(int ID) {
		std::deque <int> ::iterator futureFrontID = std::find(activeWindows.begin(), activeWindows.end(), ID);
		if (futureFrontID != activeWindows.end()) {
			windows.at(*activeWindows.begin())->setAColor(0x99);

			activeWindows.erase(futureFrontID);
			activeWindows.push_front(ID);

			windows.at(ID)->setAColor(0xFF);
		}
	};

	void Update(float dt, float mx, float my) {
		if (!activeWindows.empty()) {
			getFocusWin()->Update(dt, mx, my);
			gui->MoveCtrl(getFocusWinID(), windows[getFocusWinID()]->x, windows[getFocusWinID()]->y);

			gui->Update(dt);
			GUI_window *top = getFocusWin();
			if (mx < top->x || mx > top->x + top->w ||
				my < top->y || my > top->y + top->h) { 
					std::deque <int> ::reverse_iterator but = activeWindows.rbegin();
					while (but != activeWindows.rend()) {
						if(hgeButtonGetState(gui, *but)) {
							setFocus(*but);
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
	void Render() {
		std::deque <int> ::reverse_iterator p = activeWindows.rbegin();
		while (p != activeWindows.rend()) {
			windows.at(*p)->Render();
			p++;
		}
	};
	void setActive(int ID) {
		std::deque <int> ::iterator futureFrontID = std::find(activeWindows.begin(), activeWindows.end(), ID);
		if (futureFrontID == activeWindows.end()) {
			windows.at(ID)->visible = true;
			if (activeWindows.empty()) 
				windows.at(ID)->setAColor(0xFF); 
			else 
				windows.at(ID)->setAColor(0x99);
			activeWindows.push_back(ID);
			gui->EnableCtrl(ID, true);
		} 
	};
	void Deactive(int ID) {
		activeWindows.erase(std::find(activeWindows.begin(), activeWindows.end(), ID));
		windows.at(ID)->visible = false;
		gui->EnableCtrl(ID, false);
	}

	void setWinPos(int ID, float _x, float _y) {
		windows.at(ID)->setPos(_x, _y); 
		gui->MoveCtrl(ID, _x, _y); 
	};
};
