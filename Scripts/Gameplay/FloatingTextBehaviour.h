#pragma once

#include "Component.h"
#include <DirectXMath.h>

using namespace DirectX;

class FloatingTextBehaviour : public Component
{
public:
	FloatingTextBehaviour() = default;

	void Start() override;

};
