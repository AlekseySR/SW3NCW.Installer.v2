#ifndef __PROGCONFIG_H__
#define __PROGCONFIG_H__

#include <string>
#include <list>
#include "MainFunctions.h"

#ifdef __cplusplus
extern "C"
{
#endif

	class ProgConfig
	{

	public:

		ProgConfig() {
			SetPaths();
		}
		~ProgConfig() {
		}

		bool ParseLuaFiles(lua_State* L);

		void SetPaths() {
			this->cfgFile = "config.lua";
			this->filepath = "modinfo\\";
		}

		void CheckModVersion() {
			Print(14, "\nName:\t %s", this->ModName.c_str());
			Print(14, "Ver:\t %s", this->ModVersion.c_str());
		}

		void CheckModFiles() {
			Print(15, "Available content to install:");
			for (const auto& i : GetModFiles()) { Print(14, "%s", i.c_str()); }
		}

		void CheckUpdVersion() {
			Print(14, "CheckUpdVersion(): Under construction!");
			//ShellExecute(0, 0, L"https://www.elite-games.ru/conference/viewtopic.php?t=65403&sid=51db6023aaa54b811c9f3335773bdd46", 0, 0, SW_SHOW);
		}

		void About() {
			Print(COLOURS::YELLOW, "ASR installer (version: %.1f)", version);
			ShowCommands();
		}

		void ShowCommands() {
			Print(COLOURS::YELLOW,
				"\nCommands:\n"
				"install:\t start installation of mod files from modinfo folder,\n"
				"checkver:\t check mod version in modinfo folder which you want to install,\n"
				"checkfiles:\t check mod files in modinfo folder which are going to be installed,\n"
				"refresh:\t refresh database of modinfo,\n"
				"checkupd:\t check updates of a programm,\n"
				"exit:\t close the programm,\n"
			);
		}

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

#ifdef __cplusplus
}
#endif

#endif
