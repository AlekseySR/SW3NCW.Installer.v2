#include "main.h"
#include "ProgConfig.h"
#include "Install.h"
#include "MainFunctions.h"

#pragma comment(lib,"lua/lua54.lib")

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#endif

std::shared_ptr<ProgConfig> ProgCfg;

int lua_GetPath(lua_State* L) {
	std::string path = (char*)lua_tostring(L, 1);
	lua_pushstring(L, path.c_str());
	return 1;
}

int main(int argc, char** argv) {

	SetConsoleCtrlHandler(ConsoleHandler, TRUE);
	SetConsoleOutputCP(CP_UTF8);
	setlocale(LC_ALL, "ru");

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	bool CheckFiles;

	// переопределяем новый объект класса shared_ptr
	ProgCfg.reset(new ProgConfig());

	CheckFiles = ProgCfg->ParseLuaFiles(L);
	if (!CheckFiles) {
		Print(COLOURS::RED, "Cannot find any mod to install");
		return 0;
	}

	std::string input;
	while (std::getline(std::cin, input))
	{
		if (!input.find("install")) {
			Install(L);
		}
		else if (!input.find("checkmod")) {
			ProgCfg->CheckModVersion();
			ProgCfg->CheckModFiles();
		}
		else if (!input.find("refresh")) {
			CheckFiles = ProgCfg->ParseLuaFiles(L);
			if (!CheckFiles) {
				Print(COLOURS::RED, "cannot find any mod to install");
				return 0;
			}
			Print(COLOURS::YELLOW, "Information refreshed");
		}
		else if (!input.find("checkupd")) {
			ProgCfg->CheckUpdVersion();
		}
		else if (!input.find("about") || input.empty()) {
			ProgCfg->About();
		}
		else if (!input.find("exit")) {
			break;
		}
		else
		{
			Print(COLOURS::RED, "Error! Unknown input.");
			ProgCfg->ShowCommands();
		}
	}

	SetConsoleOutputCP(866);
	lua_close(L);

	return 0;
}

int __stdcall ConsoleHandler(DWORD dwCtrlType)
{
	switch (dwCtrlType)
	{
		//this just disables Ctrl-C
	case CTRL_C_EVENT:
		return TRUE;
		// if user pressed "X" button on console window
	case CTRL_CLOSE_EVENT:
	{

		return true;
	}
	default:
		return false;
	}
}