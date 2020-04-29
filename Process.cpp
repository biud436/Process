#include "Process.h"
#include "tlhelp32.h"

namespace Initial2D {


	Process::Process(std::wstring filename) :
		m_hWnd(NULL)
	{
		create(filename);
	}


	Process::~Process()
	{
	}

	bool Process::create(std::wstring filename)
	{
		STARTUPINFOW si;
		PROCESS_INFORMATION pi = {0,};
		ZeroMemory(&si, sizeof(STARTUPINFOW));
		ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

		si.cb = sizeof(STARTUPINFOW);
		si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
		si.dwFlags = STARTF_USESIZE | STARTF_USEPOSITION | STARTF_USESTDHANDLES;

		BOOL bRet = CreateProcessW(NULL, &filename[0], NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi);

		if (bRet == FAILED)
		{
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			fputs(catchError().c_str(), stderr);
			return false;
		}

		WaitForSingleObject(pi.hProcess, INFINITE);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		return true;
	}


	std::string Process::catchError()
	{
		DWORD errorCode = GetLastError();

		if (errorCode == 0)
			return "";

		std::wstring buf;
		buf.resize(1024);

		const WORD LCID_ENGLISH = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
		const WORD LCID_DEFAULT = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);

		DWORD dwRet = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorCode, LCID_DEFAULT, &buf[0], buf.size(), NULL);

		if (dwRet == FAILED)
		{
			dwRet = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorCode, LCID_ENGLISH, &buf[0], buf.size(), NULL);
		}

		return toMBCS(buf);

	}

	std::string Process::toMBCS(std::wstring toUTF16)
	{
		int size = WideCharToMultiByte(CP_ACP, 0, &toUTF16[0], toUTF16.size(), NULL, 0, NULL, NULL);

		std::string sFromMBCS;
		sFromMBCS.resize(size);

		WideCharToMultiByte(CP_ACP, 0, &toUTF16[0], toUTF16.size(), &sFromMBCS[0], sFromMBCS.size(), NULL, NULL);

		return sFromMBCS;
	}

}

