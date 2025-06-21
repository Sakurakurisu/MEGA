#pragma once

#include "Component.h"
#include <DirectXMath.h>
#include "Animator.h"
#include "ChargeSystem.h"
#include "PropertyTweener.h"

using namespace DirectX;

class Rigidbody2D;
class PlayerBehaviour;
class AttackInfo;
class ShakePositionTween;

class LaiSlashBehaviour : public Component
{
public:
	LaiSlashBehaviour() = default;

	void Start() override;

	void UpdateLaiSlash();

	void UpdateArrow();

	void OnDestroy() override;

	LaiSlashBehaviour& SetMinDistance(float dis);

	LaiSlashBehaviour& SetMaxDistance(float dis);

	LaiSlashBehaviour& SetChargeTime(float minTime, float maxTime);

	LaiSlashBehaviour& SetDirection(const XMFLOAT2& dir);

	LaiSlashBehaviour& SetCheckDistance(float dis);

	LaiSlashBehaviour& SetCheckAngle(float angle);

	LaiSlashBehaviour& SetDashDistance(float dis);

	void DoLaiSlash();

	void StartCharge();

	bool IsCompleted() const;

	bool ShouldJump() const;

	void Reset();

	GameObject* blackHoleEntity = nullptr;

private:
	float finalDistance = 10.0f;
	XMFLOAT2 direction = { 1.0f, 0.0f };
	float checkDistance = 10.0f;
	float checkAngle = 20.0f;

	bool isAiming = false;



	GameObject* laiSlashArrow = nullptr;

	Rigidbody2D* rb = nullptr;
	PlayerBehaviour* playerBehaviour = nullptr;

	float minDistance = 5.0f;
	float maxDistance = 10.0f;

	ChargeSystem chargeSystem{ 0.3f,1.5f };

	PropertyTweener<XMFLOAT3>* laiSlashTween = nullptr;

	std::shared_ptr<ShakePositionTween> chargeTween = nullptr;
	PropertyTweener<XMFLOAT4>* flashTween = nullptr;
	float dashDistance = 70.0f;

	bool isCompleted = false;

	bool shouldJump = false;

	XMFLOAT2 GetLaiSlashDirection() const;
	void OnBlackHoleCreated(GameObject* obj);
	void OnBlackHoleDestroy();

	void OnHit(AttackInfo attackInfo);

	void ChangeArrowEnable(bool flag);
};