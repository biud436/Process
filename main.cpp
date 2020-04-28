#define UNICODE
#define _UNICODE
#include "Process.h"
#include <fstream>

unsigned char table[177] =
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

int main(int argc, char** argv)
{
	const char* package_file = ".\\package.json";
	if (!std::ifstream(package_file)) 
	{
		std::ofstream outFile(package_file);
		outFile << table;
	}
	
	Initial2D::Process process2(L"cmd /c \"echo wow...\"");
	
	return 0;
}