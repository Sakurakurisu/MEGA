// Mover.h
#pragma once

#include "Component.h"
#include <DirectXMath.h>

using namespace DirectX;

class Mover : public Component
{
public:
	Mover() = default;

	Mover& SetMoveSpeed(float speed);

	Mover& SetDistance(float distance);

	Mover& SetMoveDir(const XMFLOAT2& dir);

	void Start() override;

	void Update() override;

	void OnTriggerEnter2D(Collision2D& other) override;

private:
	float moveSpeed = 10.0f;
	float distance = 4.0f;
	float traveledDistance = 0.0f;

	XMFLOAT3 startPos{};
	XMFLOAT2 moveDir{};

	float currentGrayMix = 1.0f;
};