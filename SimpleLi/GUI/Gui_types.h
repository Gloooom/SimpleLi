#include "Gui_inclusion.h"

#pragma once

extern HGE *hge;

struct EditorState {
	hgeVector mp;
	bool play;
	EditorState():
	play(true) {};
};

#define GUI_TEXT				0

#define CMD_WIN_S_L				1
#define CMD_WIN_ADD_IND			2
#define CMD_WIN_EDIT_PHIS		3
#define CMD_PAUSE				4

#define WIN_S_L					1
#define WIN_ADD_IND				2
#define WIN_EDIT_EYE			3
#define WIN_EDIT_PHIS			4