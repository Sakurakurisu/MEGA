#include "Movement.h"
#include "GameObject.h"
#include "keyboard.h"
#include "Animator.h"
#include "Rigidbody2D.h"
#include "AnimationClip.h"
#include "Debug.h"
#include "Input.h"
#include "GameTime.h"
#include "GravitySkillBehaviour.h"
#include "EventCenter.h"
#include "PlayerBehaviour.h"

void Movement::Start()
{
	animator = gameObject->GetComponent<Animator>();
	rb = gameObject->GetComponent<Rigidbody2D>();
	playerBehaviour = gameObject->GetComponent<PlayerBehaviour>();
}

void Movement::UpdateMove()
{
	XMFLOAT2 inputVelocity = { 0.0f, 0.0f };

	if (IsButtonPush(ButtonKind::LeftButton))
		inputVelocity.x -= 1.0f;
	//Lスティック→
	if (IsButtonPush(ButtonKind::RightButton))
		inputVelocity.x += 1.0f;

	//if (IsButtonPush(ButtonKind::
	// ))
	//	inputVelocity.y -= 1.0f;

//#ifdef _DEBUG
	if (Keyboard_IsKeyDown(KK_A))
		inputVelocity.x -= 1.0f;
	//Lスティック→
	if (Keyboard_IsKeyDown(KK_D))
		inputVelocity.x += 1.0f;

	if (Keyboard_IsKeyDown(KK_W))
		inputVelocity.y -= 1.0f;
	//Lスティック→
	if (Keyboard_IsKeyDown(KK_S))
		inputVelocity.y += 1.0f;
//#endif // DEBUG

	XMFLOAT2 currentPosition = ToXMFLOAT2(gameObject->transform.position);
	XMFLOAT2 newPosition = currentPosition + Normalize(inputVelocity) * moveSpeed * Time::GetTimeScale();

	rb->MovePosition(newPosition);

	if (inputVelocity.x > 0)
		playerBehaviour->SetFacingDirection(E_FacingDirection::Right);
	else if (inputVelocity.x < 0)
		playerBehaviour->SetFacingDirection(E_FacingDirection::Left);
}

Movement& Movement::SetSpeed(float speed)
{
	moveSpeed = speed;

	return *this;
}