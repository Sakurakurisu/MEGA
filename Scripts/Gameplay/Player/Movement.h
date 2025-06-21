#pragma once

#include "Component.h"
#include <DirectXMath.h>

class Animator;
class Rigidbody2D;
class SpriteRenderer;
class PlayerBehaviour;

using namespace DirectX;

class Movement : public Component
{
public:
	Movement() = default;

	void Start() override;

	void UpdateMove();

	Movement& SetSpeed(float speed);

private:
	float moveSpeed = 10.0f;
	Animator* animator = nullptr;
	Rigidbody2D* rb = nullptr;
	PlayerBehaviour* playerBehaviour = nullptr;
};
