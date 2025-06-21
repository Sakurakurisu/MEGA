#pragma once

#include "Component.h"
#include "AudioClip.h"

class BlackHoleBehaviour : public Component
{
public:
	BlackHoleBehaviour() = default;
	AudioClip* gravitySound;
	void Start() override;
	void OnDestroy() override;
};