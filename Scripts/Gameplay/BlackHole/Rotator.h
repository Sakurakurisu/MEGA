#pragma once

#include "Component.h"

class Rotator : public Component
{
public:
	Rotator() = default;

	Rotator& SetRotateSpeed(float speed);

	void Update() override;

private:
	float rotateSpeed = 40.0f;
};