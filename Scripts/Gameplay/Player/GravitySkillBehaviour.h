#pragma once

#include "Component.h"
#include <DirectXMath.h>
#include "ChargeSystem.h"

using namespace DirectX;

class GravitySkillBehaviour : public Component
{
public:
	GravitySkillBehaviour() = default;

	void Start() override;

	void OnDestroy() override;

	void CreateBlackHole();

	void CheckJustPoint(XMFLOAT3 pos);

	bool CanCreateBlackHole();

	void SetUsedGravity(bool flag);

	void StartCharge();

	void Update() override;
private:

	bool hasBlackHole = false;

	bool isCooling = false;

	bool usedGravity = false;

	ChargeSystem chargeSystem{ 0.3f,1.5f };

	GameObject* chargeEffect = nullptr;

	float currentGrayMix = 0.0f;

	void OnBlackHoleDestroy();

	void OnGroundCheck(GameObject* obj, bool flag);

	bool isJustPoint = false;

	bool isGrounded = false;

	GameObject* groundChecker = nullptr;

	size_t timerID;
};