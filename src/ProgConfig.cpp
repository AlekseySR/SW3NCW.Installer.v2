#include "main.h"
#include "ProgConfig.h"
#include "MainFunctions.h"

ProgConfig::ProgConfig() {
	SetPaths();
}

ProgConfig::~ProgConfig() {
}

void ProgConfig::SetPaths() {
	this->cfgFile = "config.lua";
	this->filepath = "modinfo\\";
}

void ProgConfig::CheckModInfo() {
	if (this->ModName.empty() || this->ModVersion.empty() || this->ModFiles.empty())
		throw std::exception("Main object of class has unitialized field!");

	Print(14, "\nName:\t %s", this->ModName.c_str());
	Print(14, "Ver:\t %s", this->ModVersion.c_str());
	Print(14, "Available content to install:");
	for (const auto& i : GetModFiles()) {
		Print(14, "file: %s", i.c_str());
		std::string filename = this->filepath + i;
		if (!fileExists(filename.c_str()))
		{
			std::string filename = this->filepath + i.c_str();
			Print(14, "Warning: file %s is not exist in modinfo directory!", filename);
		}
	}
}

void ProgConfig::CheckUpdVersion() {
	Print(14, "This function is under construction!");
	//ShellExecute(0, 0, L"https://www.elite-games.ru/conference/viewtopic.php?t=65403&sid=51db6023aaa54b811c9f3335773bdd46", 0, 0, SW_SHOW);
}

void ProgConfig::About() {
	Print(COLOURS::YELLOW, "ASR installer (version: %.1f)", version);
	ShowCommands();
}

void ProgConfig::ShowCommands() {
	Print(COLOURS::YELLOW,
		"\nCommands:\n"
		"install:\t start installation of mod files from modinfo folder,\n"
		"checkver:\t check mod version in modinfo folder which you want to install,\n"
		"checkfiles:\t check mod files in modinfo folder which are going to be installed,\n"
		"refresh:\t refresh database of modinfo,\n"
		"checkupd:\t check updates of a programm,\n"
		"exit:\t\t close the programm\n"
	);
}

bool ProgConfig::ParseLuaFiles(lua_State* L) {

	filename.clear();
	ModName.clear();
	ModVersion.clear();
	ModFiles.clear();

	// step 1: get file path
	if (!CheckLua(L, luaL_dofile(L, cfgFile.c_str())))
	{
		Print(COLOURS::RED, "Attempt to open non-existent or bad file.");
		return false;
	}

	lua_getglobal(L, "installer");
	if (lua_istable(L, -1)) {
		lua_pushstring(L, "modinfo");
		lua_gettable(L, -2);
		if (CheckLua(L, lua_istable(L, -1))) {
			filename = filepath + lua_tostring(L, -1);
		}
		lua_pop(L, 1);
	}

	// step 2: get file parameters
	if (!CheckLua(L, luaL_dofile(L, filename.c_str())))
	{
		Print(COLOURS::RED, "Attempt to open non-existent or bad file.");
		return false;
	}

	lua_getglobal(L, "mod");
	if (lua_istable(L, -1)) {
		lua_pushstring(L, "name");
		lua_gettable(L, -2);
		if (CheckLua(L, lua_istable(L, -1))) {
			ModName = lua_tostring(L, -1);
		}
		lua_pop(L, 1);
		if (ModName.empty()) {
			Print(4, "Got empty from LUA::mod::name field.");
			return false;
		}

		lua_pushstring(L, "version");
		lua_gettable(L, -2);
		if (CheckLua(L, lua_istable(L, -1))) {
			ModVersion = lua_tostring(L, -1);
		}
		lua_pop(L, 1);
		if (ModVersion.empty()) {
			Print(4, "Got empty from LUA::mod::version field.");
			return false;
		}

		lua_pushstring(L, "files");
		lua_gettable(L, -2);
		if (lua_istable(L, -1)) {
			lua_pushvalue(L, -1);
			lua_pushnil(L);
			while (lua_next(L, -2)) {
				lua_pushvalue(L, -2);
				//const char* key = lua_tostring(L, -1);
				//const char* value = lua_tostring(L, -2);
				//printf("%s => %s\n", key, value);
				ModFiles.push_back(lua_tostring(L, -2));
				lua_pop(L, 2);
			}
			lua_pop(L, 1);
		}
	}

	return true;
}