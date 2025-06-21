#pragma once

#include "Scene.h"
#include "Camera.h"
#include "DOTween.h"
#include "CameraFollow.h"
#include "RenderStateMgr.h"

class TestGameScene : public Scene
{
public:

	void Init() override
	{
		SetMainCamera();
		GetMainCamera()->SetOrthographic(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 10.0f);
	}
};