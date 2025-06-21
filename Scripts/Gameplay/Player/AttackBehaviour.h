#pragma once

#include "Component.h"
#include <DirectXMath.h>

using namespace DirectX;

class AttackInfo;
class Rigidbody2D;

class AttackBehaviour : public Component
{
public:
	AttackBehaviour() = default;

	void Start() override;

	void DoAttack();

	void Reset();

	AttackBehaviour& SetKnockbackForce(float force);

private:
	Rigidbody2D* rb = nullptr;
	float knockbackForce = 100.0f;

	void OnHit(AttackInfo attackInfo);
};