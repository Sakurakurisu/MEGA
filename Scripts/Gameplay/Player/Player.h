#pragma once

#include "SpriteRenderer.h"
#include "ResourceMgr.h"
#include "PlayerStates.h"
#include "Effects.h"
#include "GravitySkillBehaviour.h"
#include "ShaderSwitcher.h"
#include "ExplosionForceHandler.h"
#include "LaiSlashBehaviour.h"
#include "RigidBody2D.h"
#include "PlayerBehaviour.h"
#include "HitBoxMgr2D.h"
#include "JumpBehaviour.h"
#include "GroundChecker.h"
#include "WhirlSlashBehaviour.h"
#include "AttackBehaviour.h"
#include "RenderStateMgr.h"
#include "AfterImageComponent.h"
#include "Health.h"
#include "Suit.h"

class Player : public GameObject
{
public:
	Player(const std::string& name = "player")
		: GameObject(name)
	{
		LoadComponents();
	}

	~Player() = default;

private:
	void LoadComponents() override
	{
		SetTag(E_Tag::Player);
		SetScale(XMFLOAT3(5.0f, 5.0f, 1.0f));
		SetLayer(E_Layer::Player);

		Material meterial{};

		meterial.diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

		AddComponent<PlayerBehaviour>();

		AddComponent<SpriteRenderer>()
			.SetSprite(ResourceMgr::instance.GetAnimationClip("player_idle")->GetFrameSprite(0))
			.SetMaterial(meterial)
			.SetRenderStateConfig(RenderStateMgr::instance.GetRenderState("PlayerShaderPoint"));

		AddComponent<Movement>()
			.SetSpeed(15.0f);

		AddComponent<Animator>()
			.AddAnim(E_AnimType::Player_Idle, ResourceMgr::instance.GetAnimationClip("player_idle"))
			.AddAnim(E_AnimType::Player_Run, ResourceMgr::instance.GetAnimationClip("player_run"))
			.AddAnim(E_AnimType::Player_LaiSlashCharge, ResourceMgr::instance.GetAnimationClip("player_laiSlashCharge"))
			.AddAnim(E_AnimType::Player_LaiSlash, ResourceMgr::instance.GetAnimationClip("player_laiSlash"))
			.AddAnim(E_AnimType::Player_Attack, ResourceMgr::instance.GetAnimationClip("player_attack"))
			.AddAnim(E_AnimType::Player_WhirlSlash, ResourceMgr::instance.GetAnimationClip("player_attack2"))
			.AddFrameEvent(E_AnimType::Player_Attack, 1, [this]()
						   {
							   GetComponent<HitBoxMgr2D>()->ActiveHitBox(E_HitBoxID::Test_01);
						   })
			.AddFrameEvent(E_AnimType::Player_Attack, 7, [this]()
						   {
							   GetComponent<HitBoxMgr2D>()->DisableHitBox(E_HitBoxID::Test_01);
						   })
			.AddFrameEvent(E_AnimType::Player_LaiSlash, 1, [this]()
						   {
							   GetComponent<HitBoxMgr2D>()->ActiveHitBox(E_HitBoxID::Test_02);
						   })
			.AddFrameEvent(E_AnimType::Player_LaiSlash, 9, [this]()
						   {
							   GetComponent<HitBoxMgr2D>()->DisableHitBox(E_HitBoxID::Test_02);
						   })
			.AddFrameEvent(E_AnimType::Player_WhirlSlash, 1, [this]()
						   {
							   GetComponent<HitBoxMgr2D>()->ActiveHitBox(E_HitBoxID::Test_03);
						   })
			.AddFrameEvent(E_AnimType::Player_WhirlSlash, 7, [this]()
						   {
							   GetComponent<HitBoxMgr2D>()->DisableHitBox(E_HitBoxID::Test_03);
						   });

		AddComponent<StateMachine>()
			.AddState(E_StateType::Player_Idle, new PlayerState_Idle)
			.AddState(E_StateType::Player_Run, new PlayerState_Run)
			.AddState(E_StateType::Player_Attack, new PlayerState_Attack)
			.AddState(E_StateType::Player_Aim, new PlayerState_Aim)
			.AddState(E_StateType::Player_Shooting, new PlayerState_Shooting)
			.AddState(E_StateType::Player_GravityAffected, new PlayerState_GravityAffected)
			.AddState(E_StateType::Player_LaiSlash, new PlayerState_LaiSlash)
			.AddState(E_StateType::Player_LaiSlashCharge, new PlayerState_LaiSlashCharge)
			.AddState(E_StateType::Player_Jump, new PlayerState_Jump)
			.AddState(E_StateType::Player_WhirlSlash, new PlayerState_WhirlSlash)
			.AddState(E_StateType::Player_Fall, new PlayerState_Fall)
			.SetDefaultState(E_StateType::Player_Idle);

		AddComponent<BoxCollider2D>()
			.SetSize(0.4f, 0.33f);

		AddComponent<Rigidbody2D>()
			.SetGravityScale(2.0f);

		AddComponent<GravitySkillBehaviour>();

		//AddComponent<ShaderSwitcher>()
			//.SetTargetRenderStateConfig(RenderStateMgr::instance.GetRenderState("2DRenderAlphaBlendPointGrivatescale"));

		AddComponent<ExplosionForceHandler>()
			.SetExplosionForce(300.0f);

		AddComponent<LaiSlashBehaviour>()
			.SetMinDistance(30.0f)
			.SetMaxDistance(70.0f)
			.SetChargeTime(0.3f, 1.0f)
			.SetCheckDistance(20.0f)
			.SetCheckAngle(30.0f)
			.SetDashDistance(10.0f);

		AddComponent<HitBoxMgr2D>()
			.AddHitBox(E_HitBoxID::Test_01, 0.5f, XMFLOAT2(0.2f, 0.0f))
			.AddHitBox(E_HitBoxID::Test_02, 0.85f, XMFLOAT2(0.0f, 0.0f))
			.AddHitBox(E_HitBoxID::Test_03, 2.8f, XMFLOAT2(0.0f, 0.0f));

		AddComponent<JumpBehaviour>()
			.SetJumpVelocity(20.0f)
			.SetUpGravity(1.0f)
			.SetFallGravity(4.0f)
			.SetMaxFallSpeed(25.0f)
			.SetAirMoveSpeed(10.0f);

		AddComponent<WhirlSlashBehaviour>();

		AddComponent<AttackBehaviour>()
			.SetKnockbackForce(200.0f);

		AddComponent<AfterImageComponent>()
			.SetCount(10)
			.SetInterval(0.05f)
			.SetMinAlpha(0.1f);

		AddComponent<Suit>()
			.SetMaxEnergy(100.0f)
			.SetEnergy(100.0f);

		AddComponent<Health>()
			.SetMaxHp(100.0f)
			.SetInitHp(100.0f)
			.SetUseInvincivle(true);
	}
};