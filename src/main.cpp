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
	setlocale(LC_ALL, "ru");

	lua_State* L = luaL_newstate();
	try
	{
		luaL_openlibs(L);

		ProgCfg.reset(new ProgConfig());

		bool CheckFiles = ProgCfg->ParseLuaFiles(L);
		if (!CheckFiles) {
			Print(COLOURS::RED, "Cannot find any mod to install. Try to 'refresh'.");
		}

		std::cout << "ASRinstaller: ";
		std::string input;
		while (std::getline(std::cin, input)) {
			if (input == "install") {
				Install(L);
			}
			else if (input == "checkmod") {
				ProgCfg->CheckModInfo();
			}
			else if (input == "refresh") {
				ProgCfg->ParseLuaFiles(L);
				Print(COLOURS::YELLOW, "Information refreshed.");
			}
			else if (input == "checkupd") {
				ProgCfg->CheckUpdVersion();
			}
			else if ((input == "about") || input.empty()) {
				ProgCfg->About();
			}
			else if (input == "exit") {
				break;
			}
			else
			{
				Print(COLOURS::RED, "Unknown input.");
				ProgCfg->ShowCommands();
			}
			std::cout << "ASRinstaller: ";
		}

		lua_close(L);
		return 0;
	}
	catch (std::filesystem::filesystem_error const& ex) {
		std::cout
			<< "what():  " << ex.what() << '\n'
			<< "path1(): " << ex.path1() << '\n'
			<< "path2(): " << ex.path2() << '\n'
			<< "code().value():    " << ex.code().value() << '\n'
			<< "code().message():  " << ex.code().message() << '\n'
			<< "code().category(): " << ex.code().category().name() << '\n';
		do
		{
			Print(COLOURS::RED, "Press any key to exit.");
		} while (std::cin.get() != '\n');
		lua_close(L);
		return 1;
	}
	catch (const std::exception& ex)
	{
		Print(COLOURS::RED, ex.what());
		do
		{
			Print(COLOURS::RED, "Press any key to exit.");
		} while (std::cin.get() != '\n');
		lua_close(L);
		return 1;
	}
	catch (...)
	{
		luaL_error(L, "");
		lua_close(L);
		return 1;
	}

}

int __stdcall ConsoleHandler(DWORD dwCtrlType) {
	switch (dwCtrlType) {
		//this just disables Ctrl-C
	case CTRL_C_EVENT: {
		return TRUE;
	}
	case CTRL_CLOSE_EVENT: {
		return true;
	}
	default:
		return false;
	}
}