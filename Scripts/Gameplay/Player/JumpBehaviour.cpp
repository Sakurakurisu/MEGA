#include "JumpBehaviour.h"
#include "GameObject.h"
#include "GameTime.h"
#include "Rigidbody2D.h"
#include "GroundChecker.h"
#include "EventCenter.h"
#include "PlayerBehaviour.h"
#include "Input.h"

void JumpBehaviour::Start()
{
	rb = gameObject->GetComponent<Rigidbody2D>();
	if (rb)
		defaultGravity = rb->GetGravityScale();

	playerBehaviour = gameObject->GetComponent<PlayerBehaviour>();

	EventCenter::instance.AddEventListener<GameObject*, bool>(E_EventType::Ground_Check, this, &JumpBehaviour::OnGroundCheck);
}

void JumpBehaviour::Update()
{
	if (!rb) return;

	if (isGrounded)
	{
		canJump = true;
		canDoubleJump = true;  // 着地時に二段ジャンプをリセット
		jumpCount = 0;         // ジャンプ回数をリセット
		isJumping = false;
		hasUsedDoubleJump = false;
		rb->SetGravityScale(defaultGravity);
		auto velocity = rb->GetVelocity();
		rb->SetVelocity(XMFLOAT2(velocity.x, 0.0f));
	}
}

void JumpBehaviour::UpdateJump()
{
	if (!rb) return;

	if (!isGrounded)
	{
		isJumping = true;  // 地面にいない場合はジャンプ中とみなす
		UpdateGravityScale();
		auto velocity = rb->GetVelocity();
		if (velocity.y > maxFallSpeed)
			rb->SetVelocity(XMFLOAT2(velocity.x, maxFallSpeed));
	}
}

void JumpBehaviour::FixedUpdateJump()
{
	if (!isJumping) return;

	XMFLOAT2 inputVelocity = { 0.0f, 0.0f };

	if (IsButtonPush(ButtonKind::LeftButton))
		inputVelocity.x -= 1.0f;
	if (IsButtonPush(ButtonKind::RightButton))
		inputVelocity.x += 1.0f;

	XMFLOAT2 currentPosition = ToXMFLOAT2(gameObject->transform.position);
	XMFLOAT2 newPosition = currentPosition + Normalize(inputVelocity) * airMoveSpeed * Time::GetTimeScale();

	rb->MovePosition(newPosition);

	if (inputVelocity.x > 0)
		playerBehaviour->SetFacingDirection(E_FacingDirection::Right);
	else if (inputVelocity.x < 0)
		playerBehaviour->SetFacingDirection(E_FacingDirection::Left);
}

JumpBehaviour& JumpBehaviour::SetJumpVelocity(float velocity)
{
	jumpVelocity = velocity;
	return *this;
}

JumpBehaviour& JumpBehaviour::SetUpGravity(float scale)
{
	upGravity = scale;
	return *this;
}

JumpBehaviour& JumpBehaviour::SetFallGravity(float scale)
{
	fallGravity = scale;
	return *this;
}

JumpBehaviour& JumpBehaviour::SetMaxFallSpeed(float speed)
{
	maxFallSpeed = speed;
	return *this;
}

JumpBehaviour& JumpBehaviour::SetAirMoveSpeed(float speed)
{
	airMoveSpeed = speed;
	return *this;
}

JumpBehaviour& JumpBehaviour::SetCanDoubleJump(bool can)
{
	canDoubleJump = can;

	return *this;
}

JumpBehaviour& JumpBehaviour::SetJumpCount(int count)
{
	jumpCount = count;

	return *this;
}



bool JumpBehaviour::IsGrounded() const
{
	return isGrounded;
}

void JumpBehaviour::Jump(JumpType type)
{
	if (type == JumpType::Normal)
	{
		// 通常ジャンプのロジック
		if (canJump && jumpCount == 0)
		{
			// 一段目のジャンプ
			auto velocity = rb->GetVelocity();
			rb->SetVelocity(XMFLOAT2(velocity.x, -jumpVelocity));
			canJump = false;
			isJumping = true;
			isGrounded = false;
			jumpCount++;
			hasUsedDoubleJump = false;
		}
		else if (canDoubleJump && jumpCount == 1 && !hasUsedDoubleJump)
		{
			// 二段ジャンプ
			auto velocity = rb->GetVelocity();
			rb->SetVelocity(XMFLOAT2(velocity.x, -jumpVelocity));
			canDoubleJump = false;
			jumpCount++;
			hasUsedDoubleJump = true;
		}
	}
	else if (type == JumpType::Bounce)
	{
		auto velocity = rb->GetVelocity();
		rb->SetVelocity(XMFLOAT2(velocity.x, -jumpVelocity));
		isJumping = true;
		isGrounded = false;
		if (jumpCount == 0)
			jumpCount++;
	}
}

void JumpBehaviour::Jump(JumpType type, float vec)
{
	if (type == JumpType::Normal)
	{
		// 通常ジャンプのロジック
		if (canJump && jumpCount == 0)
		{
			// 一段目のジャンプ
			auto velocity = rb->GetVelocity();
			rb->SetVelocity(XMFLOAT2(velocity.x, -vec));
			canJump = false;
			isJumping = true;
			isGrounded = false;
			jumpCount++;
			hasUsedDoubleJump = false;
		}
		else if (canDoubleJump && jumpCount == 1 && !hasUsedDoubleJump)
		{
			// 二段ジャンプ
			auto velocity = rb->GetVelocity();
			rb->SetVelocity(XMFLOAT2(velocity.x, -vec));
			canDoubleJump = false;
			jumpCount++;
			hasUsedDoubleJump = true;
		}
	}
	else if (type == JumpType::Bounce)
	{
		auto velocity = rb->GetVelocity();
		rb->SetVelocity(XMFLOAT2(velocity.x, -vec));
		isJumping = true;
		isGrounded = false;
		if (jumpCount == 0)
			jumpCount++;
	}
}


void JumpBehaviour::UpdateGravityScale()
{
	float verticalVel = rb->GetVelocity().y;

	if (verticalVel < 0)
		rb->SetGravityScale(upGravity);
	else
		rb->SetGravityScale(fallGravity);
}

void JumpBehaviour::OnGroundCheck(GameObject* obj, bool flag)
{
	if (obj != gameObject) return;

	isGrounded = flag;
}