#include "Gui_inclusion.h"

#pragma once

extern HGE *hge;

struct EditorState {
	bool down;
	Vector <int> mp;
	bool play;
	void shutdown() {down = true;};
	EditorState():
	play(true),
	down(false) {};
};