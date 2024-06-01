#pragma once
#include <SDL.h>
#include <string>

class Image
{
public:
	SDL_Texture* tex;
	int sizeX, sizeY;
	std::string name;
	Image(SDL_Texture* t, std::string, int x, int y);
	~Image();
private:

};

