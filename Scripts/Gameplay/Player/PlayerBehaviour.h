#pragma once

#include "Component.h"
#include <DirectXMath.h>

enum class E_FacingDirection
{
	Right,
	Left
};

using namespace DirectX;

class SpriteRenderer;
class AttackInfo;
class Animator;
class Rigidbody2D;

class PlayerBehaviour : public Component
{
public:
	PlayerBehaviour() = default;

	void Start() override;

	void OnDestroy() override;

	PlayerBehaviour& SetFacingDirection(E_FacingDirection dir);

	E_FacingDirection GetFacingDirection() const;

	float CalculateTargetAngle(const XMFLOAT2& stickInput);

	void RotateToDirection(const XMFLOAT2& stickInput);

	void RotateToDirection(const XMFLOAT2& stickInput, float rotateSpeed);

	void GenerateFloatingText();

	void JustPoint(XMFLOAT3 pos);

private:
	E_FacingDirection facingDirection = E_FacingDirection::Right;

	SpriteRenderer* spriteRenderer = nullptr;

	Animator* animator = nullptr;

	Rigidbody2D* rb = nullptr;

	GameObject* lasiSlashArrow = nullptr;

	void OnAttack(AttackInfo attackInfo);

	void OnHit(AttackInfo attackInfo);
};
