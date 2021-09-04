#ifndef __PROGCONFIG_H__
#define __PROGCONFIG_H__

#include <string>
#include <vector>
#include <list>
#include "MainFunctions.h"

class ProgConfig
{
public:

	ProgConfig();
	~ProgConfig();

	void SetPaths();
	void CheckModInfo();
	void CheckUpdVersion();
	void About();
	void ShowCommands();

	bool ParseLuaFiles(lua_State* L);

	std::string GetCfgFile() { return this->cfgFile; }
	std::string GetFilePath() { return this->filepath; }
	std::string GetModName() { return this->ModName; }
	std::string GetModVersion() { return this->ModVersion; }
	std::list<std::string> GetModFiles() { return this->ModFiles; }

private:
	std::string cfgFile;
	std::string filepath;
	std::string filename;
	std::string ModName;
	std::string ModVersion;
	std::list<std::string> ModFiles;
};

#endif
