#pragma once
#include<SDL.h>
#include "WindowControl.h"
#include "FileControl.h"

namespace EventHandle {

	float wheelEvent(SDL_Event e);
	void NextEvent(FileControl* fileC, WindowControl* windowC);
	void LastEvent(FileControl* fileC, WindowControl* windowC);
	void ClickEvent(SDL_Event e, FileControl* fileC, WindowControl* windowC);
	bool HandleEvents(WindowControl* windowC, FileControl* fileC);
};
