#pragma once

#include "GameObject.h"
#include "SpriteRenderer.h"
#include "ResourceMgr.h"
#include "RenderStateMgr.h"
#include "FloatingTextBehaviour.h"

class FloatingText : public GameObject
{
public:
	FloatingText(const std::string& name = "floatingText")
		: GameObject(name)
	{
		LoadComponents();
	}

	~FloatingText() = default;

private:
	void LoadComponents() override
	{
		AddComponent<SpriteRenderer>()
			.SetSprite(ResourceMgr::instance.GetSprite("floating_text"))
			.SetRenderStateConfig(RenderStateMgr::instance.GetRenderState("2DRenderAlphaBlendPoint"))
			.enabled = true;
		AddComponent<FloatingTextBehaviour>();
	}
};
