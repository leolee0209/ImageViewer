#include "Logger.h"
#include <stdlib.h>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <thread>
#include <string>

using namespace std;

string folderPath = "C:\\Users\\User\\projects\\ImageViewer\\ImageViewer\\";
string fileName = "log.txt";
string completePath = "";
bool cutFileName = true;
int mode = ios_base::app | ios::out;


void logging(string level, string s, int line, const char* file) {
	auto t = time(nullptr);
	tm tm;
	localtime_s(&tm, &t);

	ostringstream oss;
	oss << put_time(&tm, "%m/%d %Y %H-%M-%S");
	auto timeS = oss.str();

	char* fileNameOut = nullptr;
	if (cutFileName) {
		int pathLength = strlen(file);
		for (int i = pathLength - 1; i >= 0; i--) {
			if (file[i] == '\\') {
				if (i != pathLength - 1) {
					fileNameOut = (char*)malloc(sizeof(char) * (pathLength - i));
					if (fileNameOut)
						strcpy_s(fileNameOut, (pathLength - i), &file[i + 1]);
				}
				break;
			}
		}
	}
	else {
		fileNameOut = (char*)file;
	}

	string output;
	if (fileNameOut)
		output = string(fileNameOut) + string(":") + to_string(line) + " " + timeS + level + s + "\n";
	else
		output = string("fileName_NOT_LOADED") + string(":") + to_string(line) + " " + timeS + level + s + "\n";

	if (true) {
		ofstream of;
		of.open(completePath, mode);
		of << output;
	}
	if (true) {
		cout << output;
	}
	free(fileNameOut);
}

void logWrapper(string level, string s, int line, const char* file) {
	thread t(logging, level, s, line, file);
	t.detach();
}


void note(const char* file, int line, string s) {
	logWrapper(" NOTE: ", s, line, file);
}
void note(const char* file, int line, char* s) {
	logWrapper(" NOTE: ", string(s), line, file);
}

void error(const char* file, int line, string s) {
	logWrapper(" ERROR: ", s, line, file);
}
void error(const char* file, int line, char* s) {
	logWrapper(" ERROR: ", string(s), line, file);
}

void warn(const char* file, int line, string s) {
	logWrapper(" WARNING: ", s, line, file);
}
void warn(const char* file, int line, char* s) {
	logWrapper(" WARNING: ", string(s), line, file);
}

void LoggerInit(string name, bool cut) {
	//name is initialized to "", cut is initialized to true
	if (name != "") {
		fileName = name;
	}
	cutFileName = cut;
	completePath = folderPath + fileName;
	ifstream inFile(completePath);
	int lineCount = count(istreambuf_iterator<char>(inFile),
		istreambuf_iterator<char>(), '\n');
	if (lineCount > 200) {
		mode = ios::out;
	}

}