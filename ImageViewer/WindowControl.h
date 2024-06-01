#pragma once
#include <SDL.h>
#include "FileControl.h"
#include <string>
#include "Gui.h"
#include <vector>
class FileControl;

class WindowControl
{
private:
	FileControl* fileC;
	float zoomLevel = 100.0f;
	int screenWidth = 640;
	int screenHeight = 480;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Rect* dstRect;
	SDL_Rect* srcRect;
	Gui::Label* title;

	Gui::Point zoomPoint;

	void UpdateUI();
	void ReloadRect(bool);

public:
	std::vector<Gui::Button*> buttons;

	WindowControl();
	SDL_Renderer* get_renderer();
	void UpdateScale();
	void UpdateImage();
	bool InitWindow(FileControl*);
	bool loadMedia(std::string);
	bool close();
	void changeZoom(Gui::Point, float);
	void resetZoom();
	void initUI();
};

