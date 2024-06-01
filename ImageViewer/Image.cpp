#include "Image.h"

Image::Image(SDL_Texture* t,std::string name, int x, int y)
{
	this->tex = t;
	this->name = name;
	this->sizeX = x;
	this->sizeY = y;
}

Image::~Image()
{
	SDL_DestroyTexture(tex);
}
