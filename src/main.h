#ifndef __MAIN_H__
#define __MAIN_H__

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <clocale>
#include <memory>
#include <Shlwapi.h>
#include <shlobj.h>

extern "C" {
#include "lua/include/lua.h"
#include "lua/include/lauxlib.h"
#include "lua/include/lualib.h"
}

enum COLOURS {
	STD		= 15,
	GREEN	= 2,
	RED		= 4,
	YELLOW	= 14,
};

inline const float version = 1.0;

int __stdcall ConsoleHandler(DWORD dwCtrlType);

#endif