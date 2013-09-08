#include "Gui_inclusion.h"

#pragma once

extern HGE *hge;

struct EditorState {
	hgeVector mp;
	bool play;
	EditorState():
	play(true) {};
};