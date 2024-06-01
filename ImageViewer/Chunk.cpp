#include "Chunk.h"
#include "Logger.h"
#include <stdlib.h>
#include <string>

Chunk::Chunk(int s)
{
	this->imageNum = s;
}
Chunk::~Chunk()
{
	clear();
}
bool Chunk::loaded()
{

	if (imageM.empty()) {
		return false;
	}
	for (int i = 0; i < imageNum; i++) {
		if (!imageM[i]) {
			return false;
		}
	}
	return true;
}
int Chunk::size() {
	return imageNum;
}

void Chunk::clear() {

	for (int i = 0; i < imageNum; i++) {
		delete imageM[i];
	}
	imageM.clear();

}

void Chunk::add(Image* i, int imgIndex)
{
	if (i) {
		if (imgIndex >= 0 && imgIndex < imageNum)
			imageM[imgIndex] = i;
	}
	else {
		ERROR(imgIndex + " can't be added to chunk");
	}
}

Image* Chunk::get(int index)
{
	if (index >= 0 && index < imageNum)
		return imageM[index];
	else
		return nullptr;
}
