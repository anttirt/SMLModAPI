#define MOD_LOADER_DLL

#include "GameApi.h"

#include <Windows.h>

__declspec(dllexport) void LoaderSetup() {
	HMODULE mod = LoadLibrary(L"Mod.dll");
	auto proc = GetProcAddress(mod, "ModSetup");
	typedef void(*setupfunc)();
	((setupfunc)proc)();
}