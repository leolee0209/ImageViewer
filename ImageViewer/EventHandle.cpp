#include "EventHandle.h"
#include "FileControl.h"
#include "WindowControl.h"
#include <SDL.h>
#include <iostream>
#include "Gui.h"

namespace ev = EventHandle;

bool ctrl = false;

bool EventHandle::HandleEvents(WindowControl* windowC, FileControl* fileC)
{
	bool quit = false;
	SDL_Event e;
	//Handle events on queue
	while (SDL_PollEvent(&e) != 0)
	{

		switch (e.type) {
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {
			case SDLK_ESCAPE:
				quit = true;
				break;
			case SDLK_LEFT:
				ev::LastEvent(fileC, windowC);
				break;
			case SDLK_RIGHT:
				ev::NextEvent(fileC, windowC);
				break;
			case SDLK_LCTRL:
			case SDLK_RCTRL:
				ctrl = true;
			}
			break;
		case SDL_KEYUP:
			if (e.key.keysym.sym == (SDLK_LCTRL & SDLK_RCTRL)) {
				ctrl = false;
			}
			break;
		case SDL_MOUSEWHEEL:
			if (ctrl) {
				Gui::Point zoomP = { e.wheel.mouseX,e.wheel.mouseY };
				windowC->changeZoom(zoomP, ev::wheelEvent(e));
				std::cout << e.wheel.mouseX << "," << e.wheel.mouseY << "\n";
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			ClickEvent(e, fileC, windowC);
			break;
		}
	}
	return quit;
}

float EventHandle::wheelEvent(SDL_Event e) {
	if (e.wheel.y > 0) // scroll up
	{
		return 5;
	}
	else if (e.wheel.y < 0) // scroll down
	{
		return -5;
	}
	return 0;
}

void EventHandle::NextEvent(FileControl* fileC, WindowControl* windowC) {
	fileC->NextFile();
	windowC->resetZoom();
	windowC->UpdateImage();
}
void EventHandle::LastEvent(FileControl* fileC, WindowControl* windowC) {
	fileC->LastFile();
	windowC->resetZoom();
	windowC->UpdateImage();
}
void EventHandle::ClickEvent(SDL_Event e, FileControl* fileC, WindowControl* windowC) {
	int x = e.motion.x, y = e.motion.y;
	for (int i = 0; i < windowC->buttons.size(); i++) {
		Gui::Rect r = windowC->buttons[i]->range;
		if (x >= r.x && x <= r.x + r.w && y >= r.y && y <= r.y + r.h) {
			windowC->buttons[i]->func(fileC, windowC);
			break;
		}
	}

}

