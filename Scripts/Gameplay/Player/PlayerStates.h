#pragma once

#include "BaseState.h"
#include "keyboard.h"
#include "Input.h"
#include "Debug.h"
#include "Movement.h"
#include "BlackHole.h"
#include "GravitySkillBehaviour.h"
#include "EventCenter.h"
#include "LaiSlashBehaviour.h"
#include "GameObject.h"
#include "JumpBehaviour.h"
#include "GameTime.h"
#include "RigidBody2D.h"
#include "PlayerBehaviour.h"
#include "DoTween.h"
#include "WhirlSlashBehaviour.h"
#include "AttackBehaviour.h"
#include "AfterImageComponent.h"
#include "SoundMgr.h"

class PlayerState_Idle : public BaseState
{
public:
	void Enter() override
	{
		// 待機アニメ再生
		gameObject->GetComponent<Animator>()->PlayAnimation(E_AnimType::Player_Idle);
	}

	void Update() override
	{
		// 地面から離れたら落下状態へ
		if (!gameObject->GetComponent<JumpBehaviour>()->IsGrounded())
			stateMachine->ChangeState(E_StateType::Player_Fall);

		// 移動入力があれば走り状態へ
		else if (GetLeftJoystick().x != 0.0f || GetLeftJoystick().y != 0.0f)
			stateMachine->ChangeState(E_StateType::Player_Run);

		// ジャンプ入力
		else if (IsButtonDown(ButtonKind::Button_East))
		{
			gameObject->GetComponent<JumpBehaviour>()->Jump();
			stateMachine->PushState(E_StateType::Player_Jump);
		}

		// ブラックホール照準
		else if (IsButtonDown(ButtonKind::Button_West) && gameObject->GetComponent<GravitySkillBehaviour>()->CanCreateBlackHole())
			stateMachine->ChangeState(E_StateType::Player_Aim);

		// 通常攻撃
		else if (IsButtonDown(ButtonKind::Button_North))
			stateMachine->ChangeState(E_StateType::Player_Attack);

		// 重力に引かれる状態
		else if (IsButtonPush(ButtonKind::Button_R1))
			stateMachine->ChangeState(E_StateType::Player_GravityAffected);
	}
};


class PlayerState_Run : public BaseState
{
public:
	void Enter() override
	{
		gameObject->GetComponent<Animator>()->PlayAnimation(E_AnimType::Player_Run);
	}

	void FixedUpdate() override
	{
		// 移動処理（物理ベース）
		gameObject->GetComponent<Movement>()->UpdateMove();
	}

	void Update() override
	{
		if (!gameObject->GetComponent<JumpBehaviour>()->IsGrounded())
			stateMachine->ChangeState(E_StateType::Player_Fall);

		else if (GetLeftJoystick().x == 0.0f && GetLeftJoystick().y == 0.0f)
			stateMachine->ChangeState(E_StateType::Player_Idle);

		else if (IsButtonDown(ButtonKind::Button_East))
		{
			gameObject->GetComponent<JumpBehaviour>()->Jump();
			stateMachine->PushState(E_StateType::Player_Jump);
		}

		else if (IsButtonDown(ButtonKind::Button_West) && gameObject->GetComponent<GravitySkillBehaviour>()->CanCreateBlackHole())
			stateMachine->ChangeState(E_StateType::Player_Aim);

		else if (IsButtonDown(ButtonKind::Button_North))
			stateMachine->ChangeState(E_StateType::Player_Attack);

		else if (IsButtonPush(ButtonKind::Button_R1))
			stateMachine->ChangeState(E_StateType::Player_GravityAffected);
	}
};


class PlayerState_LaiSlashCharge : public BaseState
{
public:
	void Enter() override
	{
		// ライ斬蓄力演出開始
		gameObject->GetComponent<LaiSlashBehaviour>()->StartCharge();
		gameObject->GetComponent<AfterImageComponent>()->StartAfterImage();
		gameObject->GetComponent<Animator>()->PlayAnimation(E_AnimType::Player_LaiSlashCharge);
		Time::SetTimeScale(0.5f);  // スローモーション演出
	}

	void Update() override
	{
		gameObject->GetComponent<LaiSlashBehaviour>()->UpdateLaiSlash();
		gameObject->GetComponent<LaiSlashBehaviour>()->UpdateArrow();

		// ボタン離すと発動へ
		if (IsButtonUp(ButtonKind::Button_North))
			stateMachine->ChangeState(E_StateType::Player_LaiSlash);
	}

	void Exit() override
	{
		Time::SetTimeScale(1.0f);
	}
};


class PlayerState_LaiSlash : public BaseState
{
	void Enter() override
	{
		gameObject->GetComponent<Animator>()->PlayAnimation(E_AnimType::Player_LaiSlash);
		gameObject->GetComponent<LaiSlashBehaviour>()->DoLaiSlash();
	}

	void Update() override
	{
		// スラッシュ完了
		if (gameObject->GetComponent<LaiSlashBehaviour>()->IsCompleted())
		{
			if (gameObject->GetComponent<LaiSlashBehaviour>()->ShouldJump())
			{
				gameObject->GetComponent<Animator>()->PlayAnimation(E_AnimType::Player_Idle);
				GameObject* blackHole = gameObject->GetComponent<LaiSlashBehaviour>()->blackHoleEntity;
				if (blackHole)
				{
					float signedYDis = std::abs(blackHole->transform.position.y) - std::abs(gameObject->transform.position.y);
					float YDis = std::abs(signedYDis);
					if (signedYDis < 0)
					{
						if(YDis < 0)
							gameObject->GetComponent<JumpBehaviour>()->Jump(JumpType::Bounce, 40);
						else
							gameObject->GetComponent<JumpBehaviour>()->Jump(JumpType::Bounce, YDis * 8);
					}
					else
					{
						gameObject->GetComponent<JumpBehaviour>()->Jump(JumpType::Bounce);
					}
				}
				else
				{
					gameObject->GetComponent<JumpBehaviour>()->Jump(JumpType::Bounce);
				}

				stateMachine->ChangeState(E_StateType::Player_Jump);
			}

			else
				stateMachine->ChangeState(E_StateType::Player_Idle);
		}
	}

	void Exit() override
	{
		gameObject->GetComponent<LaiSlashBehaviour>()->Reset();
		gameObject->GetComponent<AfterImageComponent>()->StopAfterImage();
		gameObject->GetComponent<Animator>()->SetPlaySpeed(1.0f);
	}
};

class PlayerState_Attack : public BaseState
{
	void Enter() override
	{
		gameObject->GetComponent<Animator>()->PlayAnimation(E_AnimType::Player_Attack);
		gameObject->GetComponent<AttackBehaviour>()->DoAttack();
	}

	void Update() override
	{
		if (!gameObject->GetComponent<Animator>()->IsPlaying())
		{
			if (IsButtonPush(ButtonKind::Button_North))
				stateMachine->ChangeState(E_StateType::Player_LaiSlashCharge);
			else
				stateMachine->ChangeState(E_StateType::Player_Idle);
		}
	}

	void Exit() override
	{
		gameObject->GetComponent<AttackBehaviour>()->Reset();
		gameObject->GetComponent<Animator>()->SetPlaySpeed(1.0f);
	}
};

class PlayerState_Aim : public BaseState
{
	void Enter() override
	{
		gameObject->GetComponent<Animator>()->PlayAnimation(E_AnimType::Player_Idle);
		gameObject->GetComponent<GravitySkillBehaviour>()->StartCharge();

		Time::SetTimeScale(0.5f);
	}

	void Update() override
	{
		gameObject->GetComponent<PlayerBehaviour>()->RotateToDirection(GetLstickVec());

		if (IsButtonUp(ButtonKind::Button_West))
		{
			DOTween::DORotation(gameObject->transform, 0.0f, 0.2f);

			stateMachine->ChangeState(E_StateType::Player_Shooting);
		}
	}

	void Exit() override
	{
		Time::SetTimeScale(1.0f);
	}
};

class PlayerState_Shooting : public BaseState
{
	void Enter() override
	{
		gameObject->GetComponent<GravitySkillBehaviour>()->CreateBlackHole();
	}

	void Update() override
	{
		if (IsButtonDown(ButtonKind::Button_West) || !gameObject->GetComponent<GravitySkillBehaviour>()->CanCreateBlackHole())
		{
			if (gameObject->GetComponent<JumpBehaviour>()->IsGrounded())
				stateMachine->ChangeState(E_StateType::Player_Idle);
			else
				stateMachine->ChangeState(E_StateType::Player_Fall);
		}
	}
};

class PlayerState_GravityAffected : public BaseState
{
	void Enter() override
	{
		gameObject->GetComponent<GravitySkillBehaviour>()->SetUsedGravity(true);
	}

	void Update() override
	{
		if (IsButtonUp(ButtonKind::Button_R1))
			stateMachine->ChangeState(E_StateType::Player_Fall);
	}
};

class PlayerState_Jump : public BaseState
{
	void FixedUpdate() override
	{
		gameObject->GetComponent<JumpBehaviour>()->FixedUpdateJump();
	}

	void Update() override
	{
		gameObject->GetComponent<JumpBehaviour>()->UpdateJump();

		if (IsButtonDown(ButtonKind::Button_East))
			gameObject->GetComponent<JumpBehaviour>()->Jump();

		else if (gameObject->GetComponent<JumpBehaviour>()->IsGrounded())
			stateMachine->ChangeState(E_StateType::Player_Idle);

		else if (IsButtonDown(ButtonKind::Button_West) && gameObject->GetComponent<GravitySkillBehaviour>()->CanCreateBlackHole())
			stateMachine->ChangeState(E_StateType::Player_Aim);

		else if (IsButtonDown(ButtonKind::Button_North))
			stateMachine->ChangeState(E_StateType::Player_WhirlSlash);

		else if (IsButtonPush(ButtonKind::Button_R1))
			stateMachine->ChangeState(E_StateType::Player_GravityAffected);
	}
};

class PlayerState_WhirlSlash : public BaseState
{
	void Enter() override
	{
		gameObject->GetComponent<Animator>()->PlayAnimation(E_AnimType::Player_WhirlSlash);
		gameObject->GetComponent<WhirlSlashBehaviour>()->DoWhirlSlash();
		gameObject->GetComponent<AfterImageComponent>()->StartAfterImage();
	}

	void FixedUpdate() override
	{
		gameObject->GetComponent<JumpBehaviour>()->FixedUpdateJump();
	}

	void Update() override
	{
		if (!gameObject->GetComponent<Animator>()->IsPlaying())
		{
			if (IsButtonPush(ButtonKind::Button_North))
				stateMachine->ChangeState(E_StateType::Player_LaiSlashCharge);

			else if (gameObject->GetComponent<WhirlSlashBehaviour>()->ShouldJump())
			{
				gameObject->GetComponent<Animator>()->PlayAnimation(E_AnimType::Player_Idle);
				gameObject->GetComponent<JumpBehaviour>()->Jump(JumpType::Bounce);
				stateMachine->ChangeState(E_StateType::Player_Jump);
			}

			else
				stateMachine->ChangeState(E_StateType::Player_Fall);
		}
	}

	void Exit() override
	{
		gameObject->GetComponent<WhirlSlashBehaviour>()->Reset();
		gameObject->GetComponent<AfterImageComponent>()->StopAfterImage();
		gameObject->GetComponent<Animator>()->SetPlaySpeed(1.0f);
	}
};

class PlayerState_Fall : public BaseState
{
	void Enter() override
	{
		gameObject->GetComponent<JumpBehaviour>()->SetCanDoubleJump(true);
		gameObject->GetComponent<JumpBehaviour>()->SetJumpCount(1);
	}

	void FixedUpdate() override
	{
		gameObject->GetComponent<Animator>()->PlayAnimation(E_AnimType::Player_Idle);
		gameObject->GetComponent<JumpBehaviour>()->FixedUpdateJump();
	}

	void Update() override
	{
		gameObject->GetComponent<JumpBehaviour>()->UpdateJump();

		if (IsButtonDown(ButtonKind::Button_East))
			gameObject->GetComponent<JumpBehaviour>()->Jump();

		else if (IsButtonDown(ButtonKind::Button_West) && gameObject->GetComponent<GravitySkillBehaviour>()->CanCreateBlackHole())
			stateMachine->ChangeState(E_StateType::Player_Aim);

		else if (gameObject->GetComponent<JumpBehaviour>()->IsGrounded())
			stateMachine->ChangeState(E_StateType::Player_Idle);

		else if (IsButtonDown(ButtonKind::Button_North))
			stateMachine->ChangeState(E_StateType::Player_WhirlSlash);
	}
};
