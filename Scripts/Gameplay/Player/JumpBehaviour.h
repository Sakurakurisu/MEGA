#pragma once

#include "Component.h"

class Rigidbody2D;
class GroundChecker;
class PlayerBehaviour;

enum class JumpType
{
	Normal,
	Bounce
};

class JumpBehaviour : public Component
{
public:
	JumpBehaviour() = default;
	~JumpBehaviour() = default;

	void Start() override;
	void Update() override;
	void UpdateJump();
	void FixedUpdateJump();

	JumpBehaviour& SetJumpVelocity(float velocity);

	JumpBehaviour& SetUpGravity(float scale);

	JumpBehaviour& SetFallGravity(float scale);

	JumpBehaviour& SetMaxFallSpeed(float speed);

	JumpBehaviour& SetAirMoveSpeed(float speed);

	JumpBehaviour& SetCanDoubleJump(bool can);
	JumpBehaviour& SetJumpCount(int count);

	void Jump(JumpType type = JumpType::Normal);

	void Jump(JumpType type, float vec);

	bool IsGrounded() const;

private:
	Rigidbody2D* rb = nullptr;
	GroundChecker* groundChecker = nullptr;
	PlayerBehaviour* playerBehaviour = nullptr;

	bool canJump = false;
	bool isJumping = false;
	float defaultGravity = 1.0f;

	bool isGrounded = false;

	float jumpVelocity = 10.0f;
	float upGravity = 0.5f;
	float fallGravity = 2.0f;
	float maxFallSpeed = 15.0f;

	float airMoveSpeed = 10.0f;

	bool canDoubleJump = false;
	int jumpCount = 0;
	const int maxJumpCount = 2;
	bool hasUsedDoubleJump = false;

	void UpdateGravityScale();

	void OnGroundCheck(GameObject* obj, bool flag);
};
