#include "main.h"
#include "ProgConfig.h"
#include "Install.h"
#include "MainFunctions.h"

extern std::shared_ptr<ProgConfig> ProgCfg;

// TODO: необходимо распараллелить задачи минимум на 2 потока с помощью библиотеки thread
// Реализовать функцию, в которую помещается список list, и в которой происходят следующие действия:
// 1. Копирование в temp-папку архива из list-а
// 2. Распаковка архива в ней
// По окончании действия поточной функции всё содержимое временной папки перенести в папку с игрой, за собой убрать мусор.

/*
	logic:
	if lua parsing function returned bad results outside installation sequence,
	we give the user try to fix it (add missed files or fix bad lua fields) and push error message to std::cout.

	But if he didn't it and started the installation sequence, we throw the exception. Mod must install correctly.
*/

void Install(lua_State* L) {

	std::string path = BrowseForFolder(
		"Choose the Star Wolves 3 Civil War path",
		"C:\\Program Files (x86)"
	);
	// if you closed the window with close buttons - back to menu
	if (path.empty()) return;

	// check if it's a game directory by check binkw32.dll existance
	std::string checkFile = path + "binkw32.dll";
	if (!fileExists(checkFile.c_str())) {
		Print(COLOURS::RED, "Attempt to start installation sequence with wrong game directory.");
		return;
	}

	bool CheckFiles = ProgCfg->ParseLuaFiles(L);
	if (!CheckFiles) {
		throw std::exception("Attempt to start installation sequence with bad parsed lua result.");
	}

	Print(COLOURS::YELLOW, "Install to: %s\n", path.c_str());

	std::string tempPath = path + "DataTemp\\";
	fs::create_directory(tempPath);

	std::string fileName, fullPath1, fullPath2;
	std::string filePath = ProgCfg->GetFilePath();
	std::list<std::string> files = ProgCfg->GetModFiles();

	const auto copyOptions = fs::copy_options::update_existing | fs::copy_options::recursive;

	for (const auto& i : files) {
		fullPath1 = filePath + i;

		if (!fileExists(fullPath1.c_str())) {
			throw std::exception("Attempt to start installation sequence with missed files.");
		}
	}

	// multi-thread logic
	auto middle = std::next(files.begin(), files.size() / 2);

	std::list<std::string> thList1(files.begin(), middle);
	std::list<std::string> thList2(middle, files.end());

	std::thread th1(InstallFiles, std::reference_wrapper(thList1), filePath, tempPath);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::thread th2(InstallFiles, std::reference_wrapper(thList2), filePath, tempPath);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	th1.join();
	th2.join();

	Print(COLOURS::STD, "Done.");

	// temp
	/*
	fs::create_directories("sandbox/dir/subdir");
	std::ofstream("sandbox/file1.txt").put('a');
	fs::copy("sandbox/file1.txt", "sandbox/file2.txt"); // copy file
	fs::copy("sandbox/dir", "sandbox/dir2"); // copy directory (non-recursive)
	fs::copy("sandbox", "sandbox_copy", copyOptions);
	static_cast<void>(std::system("tree"));

	for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(path))
	{
		std::wstring file = dirEntry.path().filename();
		std::wstring ext = dirEntry.path().filename().extension();
		std::wstring SrcPath = dirEntry.path().parent_path();
		std::wstring DestPath = SrcPath;
		DestPath.erase(0, 9);

		std::filesystem::create_directory(DestPath);

		std::wcout << SrcPath << std::endl;
		std::wcout << DestPath << std::endl;

		// check if its a file with extension, not a directory
		if (ext != L"")
		{
			std::wstring path1 = SrcPath + L"\\" + file;
			std::wstring path2 = DestPath + L"\\" + file;
			//FileCopy(path1, path2);
		}
	}
	std::filesystem::remove_all(L"DataTemp\\Data");
	*/
}

// thread method for specific container "files"
void InstallFiles(std::list<std::string>& files, std::string src, std::string dest) {
	std::string fileName, fullPath1, fullPath2;
	for (const auto& i : files) {
		fileName = i;
		Print(COLOURS::STD,
			"InstallFiles in thread: %d, \tfileName: %s", std::this_thread::get_id(), fileName.c_str()
		);

		fullPath1 = src + fileName;
		fullPath2 = dest + fileName;
		if (fileExists(fullPath2.c_str())) {
			Print(COLOURS::YELLOW, "File %s is already exist in %s folder, skipping...", fileName.c_str(), dest.c_str());
			continue;
		}
		if (fileExists(fileName.c_str())) {
			Print(COLOURS::YELLOW, "Copied %s from %s to %s", fileName.c_str(), src.c_str(), dest.c_str());
			fs::copy(src, dest);
		}

		// TODO: по плану дальше идет парсинг скопированных зип-файлов (распаковка)
		// зип-файлы после распаковки удаляются
		// Распакованный контент переносится на директорию выше, в папку с игрой -> Data
		// 
		// небольшая задержка для переключения цвета консоли
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

bool FileCopy(std::wstring& from, std::wstring& to) {
	std::ifstream SrcFile(from, std::ifstream::binary);
	std::ofstream DestFile(to, std::ifstream::binary);

	if (SrcFile) {
		// get length of file:
		SrcFile.seekg(0, SrcFile.end);
		int length = SrcFile.tellg();
		SrcFile.seekg(0, SrcFile.beg);

		char* buffer = new char[length];

		SrcFile.read(buffer, length);
		DestFile.write(buffer, length);

		SrcFile.close();
		DestFile.close();

		delete[] buffer;
		return true;
	}
	return false;
}

std::string BrowseForFolder(std::string textTitle, std::string InitPath) {
	HWND console = ::GetConsoleWindow();

	char* destbuff = new char[32768];
	std::string result = "";

	BROWSEINFOA bi = { 0 };
	ZeroMemory(&bi, sizeof(bi));

	std::string Title = textTitle;
	std::string Path = InitPath;

	bi.hwndOwner = console;
	bi.pidlRoot = 0;
	bi.pszDisplayName = (LPSTR)Title.c_str();
	bi.lpszTitle = Title.c_str();
	bi.ulFlags = BIF_EDITBOX | BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS | BIF_VALIDATE;
	bi.lpfn = &BrowsePathProc;
	bi.iImage = 0;
	bi.lParam = (LPARAM)Path.c_str();

	LPITEMIDLIST pidl = ::SHBrowseForFolderA(&bi);

	if (pidl) {
		SHGetPathFromIDListA(pidl, (LPSTR)destbuff);
		result = destbuff;
		result += "\\";
	}
	// don't forget to clean the waste
	delete[] destbuff;
	return result;
}

int __stdcall BrowsePathProc(HWND hWnd, UINT message, LPARAM lParam, LPARAM pData) {
	char* szDir = new char[32768];
	switch (message)
	{
	case BFFM_INITIALIZED: {
		LPCTSTR InitialPath = reinterpret_cast<LPCTSTR>(pData);
		SendMessage(hWnd, BFFM_SETSELECTION, TRUE, reinterpret_cast<LPARAM>(InitialPath));
		break;
	}
	case BFFM_SELCHANGED: {
		if (SHGetPathFromIDListA((LPITEMIDLIST)lParam, szDir))
			SendMessage(hWnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)szDir);
		break;
	}
	}
	delete[] szDir;
	return false;
}