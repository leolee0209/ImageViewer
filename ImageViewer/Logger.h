#pragma once
#include <string>

#ifndef LOGGER_MACRO
#define LOGGER_MACRO
#define NOTE(TEXT) note(__FILE__, __LINE__, TEXT)
#define WARN(TEXT) warn(__FILE__, __LINE__, TEXT)
#define ERROR(TEXT) error(__FILE__, __LINE__, TEXT)
#endif // LOGGER_MACRO

void note(const char* file, int line, std::string s);
void error(const char* file, int line, std::string s);
void warn(const char* file, int line, std::string s);
void note(const char* file, int line, char* s);
void error(const char* file, int line, char* s);
void warn(const char* file, int line, char* s);
void LoggerInit(std::string name = "", bool cut = true);

