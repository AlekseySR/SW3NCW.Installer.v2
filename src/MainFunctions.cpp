#include "main.h"
#include "ProgConfig.h"
#include "MainFunctions.h"

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#endif

std::string StrFormat(const std::string fmt_str, ...)
{
	int final_n, n = ((int)fmt_str.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
	std::unique_ptr<char[]> formatted;
	va_list ap;
	while (1)
	{
		formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
		strcpy(&formatted[0], fmt_str.c_str());
		va_start(ap, fmt_str);
		final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
		va_end(ap);
		if (final_n < 0 || final_n >= n)
		{
			n += abs(final_n - n + 1);
		}
		else
		{
			break;
		}
	}
	return std::string(formatted.get());
}

bool getFileContent(std::string FilePath)//, std::vector<std::string>& vecOfStrs)
{
	// Open the File
	std::ifstream in(FilePath, std::ios::in | std::ios::binary);

	// Check if object is valid
	if (!in)
	{
		std::cerr << "Cannot open the File : " << FilePath << std::endl;
		return false;
	}


	std::string str;
	// Read the next line from File untill it reaches the end.
	//while (getline(in, str))
	while (std::getline(in, str))
	{
		// Line contains string of length > 0 then save it in vector
		if (str.size() > 0)
		{
			//vecOfStrs.push_back(str);
			std::cout << str << std::endl << std::endl;
		}
	}

	//Close The File
	in.close();
	return true;
}

void Print(int colourIndex, const char* format, ...)
{
	int ColourIndex = colourIndex;
	if (!ColourIndex) ColourIndex = COLOURS::STD;

	HANDLE StdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(StdOut, (WORD)(ColourIndex));

	va_list args;
	va_start(args, format);
	vprintf(format, args);
	printf("\n");
	va_end(args);

	// set to default
	SetConsoleTextAttribute(StdOut, COLOURS::STD);
}

bool fileExists(const char* file)
{
	DWORD dwAttrib = GetFileAttributesA(file);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool CheckLua(lua_State* L, int result) {
	if (LUA_OK != result)
	{
		Print(COLOURS::RED, lua_tostring(L, -1));
		lua_pop(L, 1);
		return false;
	}
	return true;
}

// String to WString
std::wstring s2ws(const std::string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

// WString to String
std::string ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}
