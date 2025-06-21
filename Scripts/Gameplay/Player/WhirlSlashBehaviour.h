#pragma once

#include "Component.h"
#include <DirectXMath.h>
#include"AudioClip.h"

using namespace DirectX;

class AttackInfo;

class WhirlSlashBehaviour : public Component
{
public:
	WhirlSlashBehaviour() = default;

	/*void Start()override;*/

	bool ShouldJump() const;

	void DoWhirlSlash();

	void Reset();

private:
	AudioClip WhirlSlash;

	bool shouldJump = false;

	void OnHit(AttackInfo attackInfo);
};