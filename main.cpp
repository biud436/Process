#define UNICODE
#define _UNICODE
#include "Process.h"
#include <fstream>
#include <iostream>
#include <Windows.h>

#ifdef _WIN32
	#include <direct.h>
	#define CWD _getcwd
	#define CHDIR _chdir
#else
	#include "unistd.h"
	#define CWD getcwd
	#define CHDIR chdir
#endif

static void sendKey(WORD key, bool up);

unsigned char szPackageFile[177] =
{
	'\x7B', '\x0A', '\x09', '\x22', '\x6E', '\x61', '\x6D', '\x65', 
	'\x22', '\x3A', '\x20', '\x22', '\x22', '\x2C', '\x0A', '\x09', 
	'\x22', '\x6D', '\x61', '\x69', '\x6E', '\x22', '\x3A', '\x20', 
	'\x22', '\x69', '\x6E', '\x64', '\x65', '\x78', '\x2E', '\x68', 
	'\x74', '\x6D', '\x6C', '\x22', '\x2C', '\x0A', '\x09', '\x22', 
	'\x6A', '\x73', '\x2D', '\x66', '\x6C', '\x61', '\x67', '\x73', 
	'\x22', '\x3A', '\x20', '\x22', '\x2D', '\x2D', '\x65', '\x78', 
	'\x70', '\x6F', '\x73', '\x65', '\x2D', '\x67', '\x63', '\x22', 
	'\x2C', '\x0A', '\x09', '\x22', '\x77', '\x69', '\x6E', '\x64', 
	'\x6F', '\x77', '\x22', '\x3A', '\x20', '\x7B', '\x0A', '\x09', 
	'\x09', '\x22', '\x74', '\x69', '\x74', '\x6C', '\x65', '\x22', 
	'\x3A', '\x20', '\x22', '\x22', '\x2C', '\x0A', '\x09', '\x09', 
	'\x22', '\x74', '\x6F', '\x6F', '\x6C', '\x62', '\x61', '\x72', 
	'\x22', '\x3A', '\x20', '\x66', '\x61', '\x6C', '\x73', '\x65', 
	'\x2C', '\x0A', '\x09', '\x09', '\x22', '\x77', '\x69', '\x64', 
	'\x74', '\x68', '\x22', '\x3A', '\x20', '\x38', '\x31', '\x36', 
	'\x2C', '\x0A', '\x09', '\x09', '\x22', '\x68', '\x65', '\x69', 
	'\x67', '\x68', '\x74', '\x22', '\x3A', '\x20', '\x36', '\x32', 
	'\x34', '\x2C', '\x0A', '\x09', '\x09', '\x22', '\x69', '\x63', 
	'\x6F', '\x6E', '\x22', '\x3A', '\x20', '\x22', '\x69', '\x63', 
	'\x6F', '\x6E', '\x2F', '\x69', '\x63', '\x6F', '\x6E', '\x2E', 
	'\x70', '\x6E', '\x67', '\x22', '\x0A', '\x09', '\x7D', '\x0A', 
	'\x7D'
};

void sendKey(WORD key, bool up)
{
	INPUT input = {0,};
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = key;
	input.ki.dwFlags = 0;

	if(up) 
	{
		input.ki.dwFlags |= KEYEVENTF_KEYUP;
	}

	SendInput(1, &input, sizeof(input));
}

std::wstring ReadRegString(HKEY hKey, std::wstring sKey, std::wstring sValue, 
	std::wstring sDefault) {
	HKEY key;
	DWORD dwDisp;

	wchar_t wcszValue[1024];
	DWORD cbSize = sizeof(wcszValue);

	// Create key
	if (RegCreateKeyExW(hKey, sKey.data(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ, 
		NULL, &key, &dwDisp) != ERROR_SUCCESS) {
		return sDefault;
	}

	if( RegQueryValueExW(key, sValue.data(), NULL, NULL, 
		reinterpret_cast<LPBYTE>(wcszValue), &cbSize) != ERROR_SUCCESS) {
		return sDefault;
	}

	RegCloseKey(key);

	return std::wstring(wcszValue);
}

std::string ToMBCS(std::wstring wstr) {
	int size = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), wstr.size(), NULL, 0, NULL, NULL);
	if(size == 0) {
		return std::string();
	}

	std::string str;
	str.resize(size);

	WideCharToMultiByte(CP_UTF8, 0, wstr.data(), wstr.size(), &str[0], size, NULL, NULL);

	return str;
}

std::wstring ToWBCS(std::string str) {
	int size = MultiByteToWideChar(CP_UTF8, 0, str.data(), str.size(), NULL, 0);
	if(size == 0) {
		return std::wstring();
	}

	std::wstring wstr;
	wstr.resize(size);

	MultiByteToWideChar(CP_UTF8, 0, str.data(), str.size(), &wstr[0], size);

	return wstr;
}

/**
 * @brief 
 * @example 
 * main.exe <Game_FOLDER> <NWJS_FOLDER>
 * main.exe E:\Games\201907\ C:\Users\U\Desktop\MV\Windows\ToolManager\lib\v0.45.4\nwjs-sdk-v0.45.4-win-x64
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char** argv)
{		
	// Checks with command line arguments
	if(argc < 2) 
	{
		fputs("the arguments length is too short. it must be 2 or more.", stderr);
		return -1;
	}

	// Reads the registry value. 
	std::wstring sDefaultValue = L"";
	std::wstring sRet = ReadRegString(HKEY_CURRENT_USER, L"Software\\KADOKAWA\\RPGMV", L"projectFileUrl", sDefaultValue);
	
	std::wcout << sRet << std::endl;

	// Extract directory name
	size_t idx = sRet.find_last_of('/');
	size_t startPos = 8;
	std::wstring sProjectFolder = sRet.substr(startPos, idx - startPos);

	size_t i = 0;
	
	while((i = sProjectFolder.find('/')) != std::wstring::npos) {
		sProjectFolder.replace(i, 1, L"\\");
	}

	// Get command lines
	std::string projectFolder = argv[1];
	std::string nwjsFolder = argv[2];

	// Checks whether project folder is included spaces.
	if(sProjectFolder.compare(ToWBCS(projectFolder)) != 0) {
		projectFolder = ToMBCS(sProjectFolder);
	}
		
	// Change working directory.
	if(CHDIR(projectFolder.data()) == 0) 
	{
		std::cout << "Success the move directory" << std::endl;
	}
	
	// Prints out current directory.
	char *buf;
	if((buf = CWD(NULL, 0)) == 0) 
	{
		fputs("CWD error", stderr);
		return -1;		
	}
	
	// Creates the package.json if there is no that file
	const char* package_file = ".\\package.json";
	if (!std::ifstream(package_file)) 
	{
		std::ofstream outFile(package_file);
		outFile << szPackageFile;
	}

	int size = MultiByteToWideChar(CP_UTF8, 0, nwjsFolder.data(), nwjsFolder.size(), NULL, NULL);
	if(size == 0) {
		fputs("MultiByteToWideChar error", stderr);
		return -1;
	}

	// Converts with MBCS to WBCS
	std::wstring wstr;
	wstr.resize(size);

	MultiByteToWideChar(CP_UTF8, 0, nwjsFolder.data(), nwjsFolder.size(), &wstr[0], size);

	wstr += L"\\nw.exe";

	std::wcout << wstr << std::endl;
	
	// if nw.exe file exists?
	if(!std::wifstream(wstr)) {
		fputs("nw.exe is not existed!", stderr);
		return -1;
	}

	wstr += L" . test";

	SetConsoleTitle(L"PLAY-CONSOLE");
	Sleep(40);
	HWND cmd_hwnd = FindWindowW(NULL, L"PLAY-CONSOLE");

	// Auto Save it.
	int ret = MessageBoxW(cmd_hwnd, L"Do you want to save it?", L"PLAY-CONSOLE", MB_OK|MB_YESNO|MB_ICONINFORMATION);
	if(ret == IDYES) 
	{
		HWND mv_hwnd = FindWindowW(L"Qt5QWindowOwnDCIcon", NULL);
		if(mv_hwnd != NULL) 
		{
			SetForegroundWindow(mv_hwnd);
			Sleep(100);

			sendKey(VK_CONTROL, false);
			sendKey(0x53, false);
			sendKey(0x53, true);
			sendKey(VK_CONTROL, true);
		}
	}

	// Runs with the process.
	Initial2D::Process nwjs(wstr);

	return 0;
}