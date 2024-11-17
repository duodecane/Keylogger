// Loader.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "ManualMap.h"
#include <iostream>

//#define DISABLE_OUTPUT


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

bool IsCorrectTargetArchitecture(HANDLE hProc) {
	BOOL bTarget = FALSE;
	if (!IsWow64Process(hProc, &bTarget)) {
		printf("Can't confirm target process architecture: 0x%X\n", GetLastError());
		return false;
	}

	BOOL bHost = FALSE;
	IsWow64Process(GetCurrentProcess(), &bHost);

	return (bTarget == bHost);
}

DWORD GetProcessIdByName(const char* name) {
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE) {
		while (Process32Next(snapshot, &entry) == TRUE) {
			if (_stricmp(entry.szExeFile, name) == 0) {
				CloseHandle(snapshot); //thanks to Pvt Comfy
				return entry.th32ProcessID;
			}
		}
	}

	CloseHandle(snapshot);
	return 0;
}


int main()
{
	DWORD PID = 0;

	while (!PID) {
		PID = GetProcessIdByName("notepad.exe");
	}
	TOKEN_PRIVILEGES priv = { 0 };
	HANDLE hToken = NULL;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		priv.PrivilegeCount = 1;
		priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &priv.Privileges[0].Luid))
			AdjustTokenPrivileges(hToken, FALSE, &priv, 0, NULL, NULL);

		CloseHandle(hToken);
	}

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (!hProc) {
		DWORD Err = GetLastError();
		ILog("Failed to Open Process");
		system("pause");
		return 0;
	}

    //here we run the manual mapping
	std::ifstream File("XKey.dll", std::ios::binary | std::ios::ate);

	if (File.fail()) {
		printf("Opening the file failed: %X\n", (DWORD)File.rdstate());
		File.close();
		CloseHandle(hProc);
		system("PAUSE");
		return -5;
	}

	auto FileSize = File.tellg();
	if (FileSize < 0x1000) {
		printf("Filesize invalid.\n");
		File.close();
		CloseHandle(hProc);
		system("PAUSE");
		return -6;
	}

	BYTE* pSrcData = new BYTE[(UINT_PTR)FileSize];
	if (!pSrcData) {
		printf("Can't allocate dll file.\n");
		File.close();
		CloseHandle(hProc);
		system("PAUSE");
		return -7;
	}

	File.seekg(0, std::ios::beg);
	File.read((char*)(pSrcData), FileSize);
	File.close();

	printf("Mapping...\n");
	if (!ManualMapDll(hProc, pSrcData, FileSize)) {
		delete[] pSrcData;
		CloseHandle(hProc);
		printf("Error while mapping.\n");
		system("PAUSE");
		return -8;
	}
	delete[] pSrcData;

	CloseHandle(hProc);
	printf("OK\n");
	system("pause");
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
