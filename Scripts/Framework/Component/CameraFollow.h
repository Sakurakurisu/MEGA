#pragma once

#include "Component.h"
#include "GameObject.h"
#include <DirectXMath.h>

using namespace DirectX;

class CameraFollow : public Component
{
public:
	CameraFollow() = default;

	void Update() override;

	// 追従対象を設定
	CameraFollow& SetFollowTarget(GameObject* target);

	CameraFollow& SetDeadZone(bool enable);

	// デッドゾーンのサイズ（幅・高さ）を設定
	CameraFollow& SetDeadZone(float width, float height);

	// 追従の補間速度（0〜1）を設定。1なら即時移動、0.1なら遅く追従
	CameraFollow& SetFollowLerp(float lerpFactor);

private:
	GameObject* target = nullptr; // 追従対象のゲームオブジェクト

	bool useDeadZone = false;     // デッドゾーンを使うかどうか
	float deadZoneWidth = 2.0f;   // デッドゾーンの幅（ワールド単位）
	float deadZoneHeight = 2.0f;  // デッドゾーンの高さ

	float followLerp = 0.1f;      // 補間係数（追従の滑らかさ）
};
