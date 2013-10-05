#include "Gui_inclusion.h"

#pragma once

extern HGE *hge;

struct EditorState {
	bool down;
	hgeVector mp;
	bool play;
	void shutdown() {down = true;};
	EditorState():
	play(true),
	down(false) {};
};