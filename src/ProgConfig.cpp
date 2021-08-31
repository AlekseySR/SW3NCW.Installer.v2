#include "main.h"
#include "ProgConfig.h"
#include "MainFunctions.h"

bool ProgConfig::ParseLuaFiles(lua_State* L) {

	// step 1: get file path
	if (!CheckLua(L, luaL_dofile(L, cfgFile.c_str())))
		return false;

	lua_getglobal(L, "installer");
	if (lua_istable(L, -1))
	{
		lua_pushstring(L, "modinfo");
		lua_gettable(L, -2);
		if (CheckLua(L, lua_istable(L, -1)))
		{
			filename = filepath + lua_tostring(L, -1);
		}
		lua_pop(L, 1);
	}

	if (filename.empty()) {
		Print(4, "empty field detected, can't get file of modification data.");
		return false;
	}

	// step 2: get file parameters
	if (!CheckLua(L, luaL_dofile(L, filename.c_str())))
		return false;

	lua_getglobal(L, "mod");
	if (lua_istable(L, -1))
	{
		lua_pushstring(L, "name");
		lua_gettable(L, -2);
		if (CheckLua(L, lua_istable(L, -1)))
		{
			ModName = lua_tostring(L, -1);
		}
		lua_pop(L, 1);

		if (ModName.empty()) {
			Print(4, "empty field detected, can't get name of modification.");
			return false;
		}

		lua_pushstring(L, "version");
		lua_gettable(L, -2);
		if (CheckLua(L, lua_istable(L, -1)))
		{
			ModVersion = lua_tostring(L, -1);
		}
		lua_pop(L, 1);

		if (ModVersion.empty()) {
			Print(4, "empty field detected, can't get version of modification.");
			return false;
		}

		lua_pushstring(L, "files");
		lua_gettable(L, -2);
		if (lua_istable(L, -1))
		{
			lua_pushvalue(L, -1);
			lua_pushnil(L);
			while (lua_next(L, -2))
			{
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