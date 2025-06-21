#include "Rotator.h"
#include "GameTime.h"
#include "GameObject.h"

Rotator& Rotator::SetRotateSpeed(float speed)
{
	rotateSpeed = speed;

	return *this;
}

void Rotator::Update()
{
	gameObject->transform.rotation -= rotateSpeed * Time::GetDeltaTime();
}