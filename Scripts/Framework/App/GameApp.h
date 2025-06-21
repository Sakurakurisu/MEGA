#pragma once

#include "WinApp.h"

class GameApp : public WinApp
{
public:
	GameApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);

	bool Init() override;

	void StartScene() override;
	void UpdatePhysics() override;
	void UpdateScene() override;
	void DrawScene() override;
};
