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
	
	const char* projectFolder = argv[1];
	std::string nwjsFolder = argv[2];
		
	// Change working directory.
	if(CHDIR(projectFolder) == 0) 
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

	// Runs with the process.
	wstr += L" . test";
	Initial2D::Process nwjs(wstr);

	return 0;
}