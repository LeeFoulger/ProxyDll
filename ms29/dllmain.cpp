#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NON_CONFORMING_SWPRINTFS
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <codecvt>

#include "utils/Console.hpp"
#include "utils/Utils.hpp"
#include "Patch.hpp"

#include "classes/camera.hpp"
#include "classes/tags.hpp"
#include "classes/ui.hpp"

#include "memory/local_types.hpp"

int AssignHotkeys(float sleep_time = 0.5)
{
	while (true)
	{
		//AssignHotkey(VK_F2, &SetAllQualitySettingsLow);

		Sleep((int)(sleep_time * 1000));
	}
	return 0;
}

int MainThread()
{
	ConfigManager = { ".\\bin\\ms29.ini" };

	UnprotectModuleMemory();
	Sleep(1000);

	g_use_default_system_ui_language = ConfigManager.GetBool("Language", "UseSystemDefault");
	g_new_system_ui_language = ConfigManager.GetLanguage("Language", "Selected");

	AddCameraHooks("Camera");
	AddCameraPatches("Camera");
	AddTagsHooks("Tags");
	AddTagsPatches("Tags");
	AddUiHooks("Ui");
	AddUiPatches("Ui");

	ApplyHooks();
	ApplyPatches();

	Sleep(1000);
	// BACKEND_SESSION enum patch
	while ((*(char*)0x3254113 == '0') /*|| *(char*)0x3254151 == '1' || *(char*)0x325418F == '2'*/ || (*(char*)0x32541CD == '3'))
	{
		*(char*)0x3254113 = '3'; // var BACKEND_SESSION_OFFLINE                  : int    = 0
		//*(char*)0x3254151 = '1'; // var BACKEND_SESSION_AUTHENTICATING           : int    = 1
		//*(char*)0x325418F = '2'; // var BACKEND_SESSION_ESTABLISHING             : int    = 2
		*(char*)0x32541CD = '0'; // var BACKEND_SESSION_ONLINE                   : int    = 3
	}

	return AssignHotkeys(1.5);
}

BOOL InitInstance(HINSTANCE hModule)
{
	SetProcessDPIAware();
	DisableThreadLibraryCalls(hModule);

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&MainThread, NULL, 0, NULL);

	return true;
}

BOOL ExitInstance(HINSTANCE hModule)
{
	FreeLibrary(hModule);
	return true;
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hModule, DWORD Reason, LPVOID Misc)
{
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH: return InitInstance(hModule);
	case DLL_PROCESS_DETACH: return ExitInstance(hModule);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		return true;
	}
}
