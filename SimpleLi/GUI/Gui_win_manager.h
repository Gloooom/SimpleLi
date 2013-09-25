#include "Gui_types.h"
#include "Gui_window.h"
#include "Gui_color.h"

#define GetGuiButton(manager, winid, ctrlname) ((hgeGUIButton*)(manager->getGui(winid)->getCtrl(ctrlname)))
#define GetGuiSlider(manager, winid, ctrlname) ((hgeGUISlider*)(manager->getGui(winid)->getCtrl(ctrlname)))
#define GetGuiListbox(manager, winid, ctrlname) ((hgeGUIListbox*)(manager->getGui(winid)->getCtrl(ctrlname)))

#define GetGuiButtonState(manager, winid, ctrlname) (((hgeGUIButton*)(manager->getGui(winid)->getCtrl(ctrlname)))->GetState())
#define GetGuiSliderValue(manager, winid, ctrlname) (((hgeGUISlider*)(manager->getGui(winid)->getCtrl(ctrlname)))->GetValue())
#define GetGuiListboxSelect(manager, winid, ctrlname) (((hgeGUIListbox*)(manager->getGui(winid)->getCtrl(ctrlname)))->GetSelectedItem())

#define SetGuiButtonState(manager, winid, ctrlname, state) (((hgeGUIButton*)(manager->getGui(winid)->getCtrl(ctrlname)))->SetState(state))
#define SetGuiSliderValue(manager, winid, ctrlname, val) (((hgeGUISlider*)(manager->getGui(winid)->getCtrl(ctrlname)))->SetValue(val))
#define SetGuiListboxSelect(manager, winid, ctrlname, select) (((hgeGUIListbox*)(manager->getGui(winid)->getCtrl(ctrlname)))->SetSelectedItem(select))
#define SetGuiText(manager, winid, ctrlname, text) (((hgeGUIText*)(manager->getGui(winid)->getCtrl(ctrlname)))->SetText(text));

#define SetLink(manager, winID, sliderID, textID) SetGuiText(manager, winID, textID, Convert(GetGuiSliderValue(winManager, winID, sliderID)).c_str())
#define SetValLink(manager, winID, value, textID) SetGuiText(manager, winID, textID, Convert(value).c_str())

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
