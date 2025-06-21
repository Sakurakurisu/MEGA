#pragma once

#include <string>
#include <Windows.h>
#include "CpuTimer.h"

class WinApp
{
public:
	WinApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);

	HWND GetMainWnd() const;
	HINSTANCE GetAppInst() const;
	float GetAspectRatio() const;

	int Run();

	virtual bool Init();
	//virtual void OnResize();

	virtual void StartScene() = 0;
	virtual void UpdatePhysics() = 0;
	virtual void UpdateScene() = 0;
	virtual void DrawScene() = 0;
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	bool InitMainWindow(); // メインウィンドウの初期化
	void CalculateFrameStats(); // フレーム統計情報の計算

	CpuTimer cpuTimer;

	HINSTANCE hAppInst = nullptr;
	HWND hMainWnd = nullptr;
	bool appPaused = false; // アプリが一時停止中かどうか

	std::wstring mainWndCaption;
	int clientWidth;
	int clientHeight;
};
