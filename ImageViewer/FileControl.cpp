#include "FileControl.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <cctype>
#include "Logger.h"
#include <thread>
#include <SDL.h>
#include <SDL_image.h>
#include <cmath>

using namespace std;
namespace fs = filesystem;


FileControl::FileControl(WindowControl* windowC) {
	this->windowC = windowC;
	folderPath = "";
	curFileIndex = 0;
	indexInChunk = 0;
	chunkIndex = 0;
}

void FileControl::InitFile(char* path)
{
	string filePath(path);
	string curFileName;
	for (int i = filePath.length() - 1; i >= 0; i--) {
		if (filePath[i] == '\\') {
			folderPath = filePath.substr(0, i);
			curFileName = filePath.substr(i + 1, filePath.length() - i - 1);
			break;
		}
	}
	ReloadFilePaths();
	for (int i = 0; i < filePaths.size(); i++) {
		if (filePaths[i] == curFileName) {
			curFileIndex = i;
			break;
		}
	}
	loadChunk(0);
}

void FileControl::ReloadFilePaths()
{
	filePaths.clear();
	int i = 0;
	for (const auto& entry : fs::directory_iterator(fs::u8path(folderPath))) {
		auto tolowercase = [](string* s)->string {
			for (int i = 0; i < s->size(); i++) {
				(*s)[i] = tolower((*s)[i]);
			}
			return *s;
			};
		std::string ex = tolowercase(&entry.path().extension().string());
		if (ex == ".png" || ex == ".jpg") {
			string fileName = entry.path().filename().string();
			i++;
			filePaths.push_back(entry.path().filename().string());
		}
	}
	NOTE(to_string(i) + " files listed.");

	for (auto& c : chunkV) {
		delete c;
	}
	chunkV.clear();
	const int chunkSize = Chunk::chunkSize;
	int chunkNum = ceil((double)filePaths.size() / chunkSize);
	int lastChunkSize = filePaths.size() - (chunkNum - 1) * chunkSize;
	for (int i = 0; i < chunkNum; i++) {
		chunkV.push_back(new Chunk((i == chunkNum - 1) ? lastChunkSize : chunkSize));
	}

}

Image* FileControl::GetCurrentImage()
{
	return curChunk()->get(indexInChunk);
}

void FileControl::LoadImages(Chunk* chunk, int index)
{
	int size = chunk->size();
	auto loadToChunk = [](FileControl* fc, string name, Chunk* chunk, int imgIndex) {
		chunk->add(fc->loadMedia(name), imgIndex);
		};

	vector<thread*> t;
	for (int i = 0; i < size; i++) {
		t.push_back(new thread(loadToChunk, this, filePaths[index * Chunk::chunkSize + i], chunk, i));
	}
	for (int i = 0; i < size; i++) {
		t[i]->join();
	}
	for (int i = 0; i < size; i++) {
		delete t[i];
	}
	NOTE("" + to_string(index) + ". chunk is loaded.");
}

string FileControl::NextFile()
{
	advance(1);
	return folderPath + '\\' + filePaths[curFileIndex];
}

string FileControl::LastFile()
{
	advance(-1);
	return folderPath + '\\' + filePaths[curFileIndex];
}

void FileControl::close()
{
	for (auto& c : chunkV) {
		delete c;
	}
}

string FileControl::GetCurrentPath() {
	return folderPath + '\\' + filePaths[curFileIndex];
}

string FileControl::GetCurrentName()
{
	return GetCurrentImage()->name;
}

Image* FileControl::loadMedia(string name)
{
	if (name == "") {
		return nullptr;
	}
	string mediaPath = folderPath + '\\' + name;
	SDL_Texture* imageTexture = IMG_LoadTexture(windowC->get_renderer(), mediaPath.c_str());
	if (imageTexture == NULL)
	{
		WARN("Failed to load image! Path: " + mediaPath);
		return nullptr;
	}

	Image* image = new Image(imageTexture, name, 0, 0);

	SDL_QueryTexture(imageTexture, NULL, NULL, &image->sizeX, &image->sizeY);

	return image;
}
void FileControl::adFileIndex(int advance) { //advance files
	if (curFileIndex + advance >= (int)filePaths.size()) { //go around to the front
		curFileIndex = 0;
	}
	else if (curFileIndex + advance < 0) { // go to the back
		curFileIndex = filePaths.size() - 1;
	}
	else { // regular
		curFileIndex += advance;
	}
}
void FileControl::adChunkIndex(int advance) { //advance chunks
	if (chunkIndex + advance >= (int)chunkV.size()) { //go around to the front
		chunkIndex = 0;
	}
	if (chunkIndex + advance < 0) { // go to the back
		chunkIndex = chunkV.size() - 1;
	}
	else { // regular
		chunkIndex += advance;
	}
}
void FileControl::loadChunk(int advance) {
	if (indexInChunk + advance < 0) { // go to the back
		curChunk()->clear();

		adChunkIndex(-1);
		indexInChunk = curChunk()->size() - 1;
		LoadImages(curChunk(), chunkIndex);
	}
	else if (indexInChunk + advance >= curChunk()->size()) { //go around to the front
		curChunk()->clear();

		adChunkIndex(1);
		indexInChunk = 0;
		LoadImages(curChunk(), chunkIndex);
	}
	else { //not advancing chunks
		indexInChunk += advance;
		if (!curChunk()->loaded()) {
			LoadImages(curChunk(), chunkIndex);
		}
	}
}

void FileControl::advance(int num) {
	adFileIndex(num);
	loadChunk(num);
}

Chunk* FileControl::curChunk() {
	return chunkV[chunkIndex];
}
