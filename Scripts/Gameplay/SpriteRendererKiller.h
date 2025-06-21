#pragma once

#include "Component.h"

class GameObject;

class SpriteRendererKiller : public Component
{
public:
	SpriteRendererKiller() = default;

	void Start() override;

	void Update() override;
private:
	GameObject* player = nullptr;
	GameObject* camera = nullptr;
};
