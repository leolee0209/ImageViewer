#pragma once
#include <vector>
#include "Image.h"
#include <map>

class Chunk
{
private:
	int imageNum;
	std::map<int, Image*> imageM;
public:
	static const int chunkSize = 5;
	static const int easeIn = 3;
	Chunk(int size);
	~Chunk();
	bool loaded();
	int size();
	void clear();
	void add(Image*, int);
	Image* get(int index);
};