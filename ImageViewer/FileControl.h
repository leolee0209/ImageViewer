#pragma once
#include <string>
#include <vector>
#include "Image.h"
#include "WindowControl.h"
#include "Chunk.h"

class WindowControl;


class FileControl
{
private:
	std::string folderPath;
	std::vector<std::string> filePaths;
	std::vector<Chunk*> chunkV;
	WindowControl* windowC;
	int curFileIndex;
	int indexInChunk;
	int chunkIndex;

	inline std::string GetCurrentPath();
	Image* loadMedia(std::string);
	void adFileIndex(int advance);
	void adChunkIndex(int advance);
	Chunk* curChunk();
	void advance(int num);
	void loadChunk(int advance);
public:
	std::string GetCurrentName();
	void InitFile(char*);
	FileControl(WindowControl*);
	void ReloadFilePaths();
	std::string NextFile();
	std::string LastFile();
	void LoadImages(Chunk* chunk, int index);
	Image* GetCurrentImage();
	void close();
};

