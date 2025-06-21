#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "GameApp.h"
#include "DXTrace.h"
#include <shellscalingapi.h>
#include "DOTween.h"
#include <iostream>
#include <functional>
#include <thread>
#include <chrono>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment(lib, "Shcore.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "Dwrite.lib")
#pragma comment(lib, "dxgi.lib")
float globalValue = 10.0f;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevInstance, _In_ LPSTR cmdLine, _In_ int showCmd)
{
	HR(SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE));
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);
	UNREFERENCED_PARAMETER(showCmd);
#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(123);
	//Debug::EnableConsole();
#endif

	GameApp theApp(hInstance, L"MEGA", 1920, 1080);
	if (!theApp.Init())
		return 0;
	//GameApp::instance.Uninit();
	return theApp.Run();
}