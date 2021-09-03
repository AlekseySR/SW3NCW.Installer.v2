#include "main.h"
#include "ProgConfig.h"
#include "MainFunctions.h"

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
			luaL_error(L, "");
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