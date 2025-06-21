#include "CameraFollow.h"
#include "GameObject.h"
#include "GameTime.h"   
#include <cstdlib> 
#include <cmath>

CameraFollow& CameraFollow::SetFollowTarget(GameObject* target)
{
	this->target = target;
	return *this;
}

CameraFollow& CameraFollow::SetDeadZone(bool enable)
{
	useDeadZone = enable;
	return *this;
}

CameraFollow& CameraFollow::SetDeadZone(float width, float height)
{
	deadZoneWidth = width;
	deadZoneHeight = height;
	return *this;
}

CameraFollow& CameraFollow::SetFollowLerp(float lerpFactor)
{
	followLerp = lerpFactor;
	return *this;
}


void CameraFollow::Update()
{
	if (!target) return; // 追従対象が未設定なら何もしない

	XMFLOAT3 camPos = gameObject->transform.position;   // 現在のカメラ位置
	XMFLOAT3 targetPos = target->transform.position;    // ターゲットの位置

	float offsetX = targetPos.x - camPos.x;
	float offsetY = targetPos.y - camPos.y;

	float deltaX = 0.0f, deltaY = 0.0f;

	if (useDeadZone)
	{
		// デッドゾーンの半分のサイズ
		float deadHalfWidth = deadZoneWidth * 0.5f;
		float deadHalfHeight = deadZoneHeight * 0.5f;

		// ターゲットがデッドゾーンの外に出た場合のみ、カメラを動かす
		if (offsetX > deadHalfWidth)
			deltaX = offsetX - deadHalfWidth;
		else if (offsetX < -deadHalfWidth)
			deltaX = offsetX + deadHalfWidth;

		if (offsetY > deadHalfHeight)
			deltaY = offsetY - deadHalfHeight;
		else if (offsetY < -deadHalfHeight)
			deltaY = offsetY + deadHalfHeight;
	}
	else
	{
		// デッドゾーンを使わない場合は、完全追従
		deltaX = offsetX;
		deltaY = offsetY;
	}

	// カメラが向かうべき新しい位置
	float targetCamX = camPos.x + deltaX;
	float targetCamY = camPos.y + deltaY;

	// 線形補間（Lerp）で滑らかに移動
	float newPosX = camPos.x + (targetCamX - camPos.x) * followLerp;
	float newPosY = camPos.y + (targetCamY - camPos.y) * followLerp;

	camPos.x = newPosX;
	camPos.y = newPosY;
	gameObject->transform.position = camPos; // カメラの位置更新
}