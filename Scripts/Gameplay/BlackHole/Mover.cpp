// Mover.cpp
#include "Mover.h"
#include "GameTime.h"
#include "GameObject.h"
#include "EventCenter.h"
#include "Collision2D.h"
#include "DOTween.h"

Mover& Mover::SetMoveSpeed(float speed)
{
	moveSpeed = speed;
	return *this;
}

Mover& Mover::SetDistance(float distance)
{
	this->distance = distance;
	return *this;
}

Mover& Mover::SetMoveDir(const XMFLOAT2& dir)
{
	moveDir = dir;
	return *this;
}

void Mover::Start()
{
	startPos = gameObject->transform.position;
	traveledDistance = 0.0f;
}

void Mover::Update()
{
	if (traveledDistance < distance)
	{
		float deltaMove = moveSpeed * Time::GetDeltaTime();
		gameObject->transform.position += moveDir * deltaMove;
		traveledDistance += deltaMove;
	}
	else
	{
		DOTween::To<float>(
			[this]() -> float
			{
				return currentGrayMix;
			},
			[this](const float& val)
			{
				currentGrayMix = val;
			},
			0.0f,
			0.3f)
			->OnUpdateValue([this](const float& newVal)
							{
								BasicEffect::instance.SetGrayMix(newVal);
							})
			->OnComplete([this]()
						 {
							 currentGrayMix = 1.0f;
						 })
			->SetEase(EaseType::QuadOut)
			->Play();
		Destroy(gameObject);
	}

}

void Mover::OnTriggerEnter2D(Collision2D& other)
{
	if (other.gameObject.layer == E_Layer::Ground)
	{
		float dot = moveDir.x * other.normal.x + moveDir.y * other.normal.y;
		moveDir.x = moveDir.x - 2.0f * dot * other.normal.x;
		moveDir.y = moveDir.y - 2.0f * dot * other.normal.y;
	}
}