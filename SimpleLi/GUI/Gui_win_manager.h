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
#define SetWinText(manager, winid, ctrlname, text) (((hgeGUIText*)(manager->getWin(winid)->getCtrl(ctrlname)))->SetText(text));

#define SetLink(manager, winID, sliderID, textID) SetWinText(manager, winID, textID, Convert(GetWinSliderValue(winManager, winID, sliderID)).c_str())
#define SetValLink(manager, winID, value, textID) SetWinText(manager, winID, textID, Convert(value).c_str())

class GUI_win_manager {
private:
	std::map <int, GUI_window*> windows;
	std::deque <int> activeWindows;
	hgeGUI *gui;
	HTEXTURE diaTex;
public:
	GUI_win_manager();

	void addWindow(GUI_window *win, int ID, float startPosX = 0, float startPosY = 0);
	void setWinPos(int ID, float _x, float _y);

	int getFocus();
	void setFocus(int ID);
	
	GUI_window *getFocusWin();
	GUI_window *getWin(int ID);

	void Update(float dt, float mx, float my);
	void Render();

	void Activate(int ID);
	void Deactivate(int ID);

	bool checkHit(int mp_x, int mp_y);
};
