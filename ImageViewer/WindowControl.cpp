#include "Gui.h"
#include "Logger.h"
#include "WindowControl.h"
#include "EventHandle.h"
#include <iostream>
#include "Image.h"
#include <SDL_image.h>
#include <SDL_ttf.h>

using namespace std;
using namespace Gui;
namespace ev = EventHandle;

Color titleC = { 45, 52, 54 };
Color secondC = { 255,255,255 };
Color black = { 0,0,0 };

int titleHeight = 40;
int slideRangeWidth = 300;

WindowControl::WindowControl() {
	window = nullptr;
	renderer = nullptr;
	dstRect = nullptr;
	srcRect = nullptr;
	title = nullptr;
	fileC = nullptr;
	zoomPoint.x = 0;
	zoomPoint.y = 0;
}

bool WindowControl::InitWindow(FileControl* fileC) {
	this->fileC = fileC;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		ERROR("SDL could not initialize! SDL_Error: " + string(SDL_GetError()));
	}

	//Create window
	window = SDL_CreateWindow("ImageViewer",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		0, 0,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
	if (!window)
	{
		ERROR("Window could not be created! SDL_Error: " + string(SDL_GetError()));
	}
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	screenWidth = DM.w;
	screenHeight = DM.h;

	//Create renderer
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		ERROR("Renderer could not be created! SDL_Error: " + string(SDL_GetError()));
	}

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		ERROR("SDL_image could not initialize! SDL_image Error: " + string(IMG_GetError()));
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

	//init ttf
	if (TTF_Init() < 0) {
		ERROR("SDL_ttf could not initialize! Error: " + string(TTF_GetError()));
	}

	initUI();

	return true;
}


void WindowControl::UpdateScale()
{
	ReloadRect(false);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, fileC->GetCurrentImage()->tex, srcRect, dstRect);

	UpdateUI();

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderPresent(renderer);
}

void WindowControl::UpdateImage() {
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	ReloadRect(true);
	Image* curImage = fileC->GetCurrentImage();
	if (curImage) {
		SDL_RenderCopy(renderer, fileC->GetCurrentImage()->tex, srcRect, dstRect);
	}

	UpdateUI();

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderPresent(renderer);
}

void WindowControl::UpdateUI() {
	//title
	title->text = fileC->GetCurrentName();
	title->draw(renderer);
	//button
	for (auto& b : buttons) {
		b->draw(renderer);
	}
}



void WindowControl::ReloadRect(bool fullSize) {
	if (dstRect == nullptr) {
		dstRect = new SDL_Rect();
	}
	if (srcRect == nullptr) {
		srcRect = new SDL_Rect();
	}

	Image* image = fileC->GetCurrentImage();
	if (image == nullptr) {
		ERROR("Current Image Is NULLPTR");
		return;
	}
	SDL_Point size = { image->sizeX,image->sizeY };

	SDL_Point oriZoomP = { size.x / 2,size.y / 2 };
	if (fullSize) {
		srcRect->w = size.x;
		srcRect->h = size.y;
		srcRect->x = 0;
		srcRect->y = 0;

	}
	else {
		srcRect->w = (double)size.x * 100 / zoomLevel;
		srcRect->h = (double)size.y * 100 / zoomLevel;
		if (zoomPoint.x >= dstRect->x || zoomPoint.x <= dstRect->x + dstRect->w ||
			zoomPoint.y >= dstRect->y || zoomPoint.y <= dstRect->y + dstRect->h) {
			oriZoomP.x = (double)(zoomPoint.x - dstRect->x) / dstRect->w * size.x;
			oriZoomP.y = (double)(zoomPoint.y - dstRect->y) / dstRect->h * size.y;
		}
		else if (zoomLevel == 100) {
			oriZoomP.x = size.x / 2;
			oriZoomP.y = size.y / 2;
		}

		srcRect->x = oriZoomP.x - srcRect->w / 2;
		srcRect->y = oriZoomP.y - srcRect->h / 2;
	}
	dstRect->w = (double)size.x / size.y * (screenHeight - titleHeight);
	dstRect->h = screenHeight - titleHeight;
	dstRect->x = (screenWidth - dstRect->w) / 2;
	dstRect->y = titleHeight;

	char* o = (char*)malloc(sizeof(char) * 50);
	snprintf(o, 50, "zoomLevel: %.1f%c", zoomLevel, '%');
	NOTE(o);
	snprintf(o, 50, "size: %f", ((double)size.x / size.y));
	NOTE(o);
	snprintf(o, 50, "dstRect: %f", ((double)dstRect->w / dstRect->h));
	NOTE(o);
	snprintf(o, 50, "srcRect: %f\n", ((double)srcRect->w / srcRect->h));
	NOTE(o);
	free(o);
}

bool WindowControl::close()
{
	SDL_DestroyRenderer(renderer);
	delete dstRect;
	delete title;
	for (auto p : buttons) {
		delete p;
	}
	buttons.clear();
	SDL_DestroyWindow(window);
	TTF_Quit();
	CloseGui();
	SDL_Quit();
	IMG_Quit();
	return true;
}

SDL_Renderer* WindowControl::get_renderer()
{
	return renderer;
}

void WindowControl::changeZoom(Point clickP, float num) {
	if (zoomLevel + num >= 100) {

		zoomPoint.x = clickP.x - dstRect->x;
		zoomPoint.y = clickP.y - dstRect->y;
		zoomLevel += num;
	}
	else {
		zoomLevel = 100;
	}
	UpdateScale();
}

void WindowControl::resetZoom() {
	zoomLevel = 100;
}

void WindowControl::initUI()
{
	TTF_Font* font = TTF_OpenFont("NotoSans-VariableFont_wdth,wght.ttf", 30);

	InitGui(font);

	Rect titleRect = { 0,0,screenWidth,titleHeight };
	Rect titleWordRect = { 0,0,100,30 };
	title = new Label(titleRect, titleWordRect, "title", titleC);

	Rect slideLR = { 10, screenHeight / 2, 30, 80 };
	Rect slideLWR = { 10, screenHeight / 2, 30, 80 };
	Rect slideLRange = { 0, 0, slideRangeWidth, screenHeight };
	auto label = new Label(slideLR, slideLWR, "<", secondC);
	auto slideL = new Button(slideLRange, ev::LastEvent);
	slideL->addKid(label);

	Rect slideRR = { screenWidth - 40, screenHeight / 2, 30, 80 };
	Rect slideRWR = { screenWidth - 40, screenHeight / 2, 30, 80 };
	Rect slideRRange = { screenWidth - slideRangeWidth, 0, slideRangeWidth, screenHeight };
	auto label2 = new Label(slideRR, slideRWR, ">", secondC);
	auto slideR = new Button(slideRRange, ev::NextEvent);
	slideR->addKid(label2);

	Rect pickRange = { 10,10,30,30 };
	auto pickLabel = new Label(pickRange, pickRange, "", secondC);
	auto pickB = new Button(pickRange, NULL);
	pickB->addKid(pickLabel);
	title->addKid(pickB);

	buttons.push_back(slideL);
	buttons.push_back(slideR);

}
