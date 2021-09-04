#ifndef __MAINFUNCTIONS_H__
#define __MAINFUNCTIONS_H__

#include <codecvt>

std::string StrFormat(const std::string fmt_str, ...);
void Print(int colourIndex, const char* format, ...);
bool fileExists(const char* file);
bool CheckLua(lua_State* L, int result);
std::wstring s2ws(const std::string& str);
std::string ws2s(const std::wstring& wstr);

#endif
