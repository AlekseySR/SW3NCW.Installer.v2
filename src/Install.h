#ifndef __INSTALL_H__
#define __INSTALL_H__

#include <thread>
#include <chrono>
#include "main.h"
#include "ProgConfig.h"

namespace fs = std::filesystem;

void InstallFiles(std::list<std::string>& files, std::string src, std::string dest);
void Install(lua_State* L);
std::string BrowseForFolder(std::string textTitle, std::string InitPath);
int __stdcall BrowsePathProc(HWND hWnd, UINT message, LPARAM lParam, LPARAM pData);

#endif