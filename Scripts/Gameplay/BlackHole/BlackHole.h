#pragma once

#include "GameObject.h"
#include "SpriteRenderer.h"
#include "ResourceMgr.h"
#include "Collider2D.h"
#include "BlackHoleState.h"
#include "Input.h"
#include "Rotator.h"
#include "Gravity.h"
#include "Mover.h"
#include "BlackHoleBehaviour.h"
#include "RigidBody2D.h"
#include "RenderStateMgr.h"

class BlackHole : public GameObject
{
public:
	BlackHole(const std::string& name = "black_hole")
		: GameObject(name)
	{
		LoadComponents();
	}

	~BlackHole() = default;

private:
	void LoadComponents() override
	{
		SetLayer(E_Layer::BlackHole);
		SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));

		Material meterial{};

		meterial.diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

		AddComponent<SpriteRenderer>()
			.SetSprite(ResourceMgr::instance.GetAnimationClip("blackHole_origin")->GetFrameSprite(0))
			.SetMaterial(meterial)
			.SetRenderStateConfig(RenderStateMgr::instance.GetRenderState("PlayerShaderPoint"));

		AddComponent<Animator>()
			.AddAnim(E_AnimType::BlackHole_Origin, ResourceMgr::instance.GetAnimationClip("blackHole_origin"))
			.AddAnim(E_AnimType::BlackHole_Expand, ResourceMgr::instance.GetAnimationClip("blackHole_expend"));

		AddComponent<Rigidbody2D>()
			.SetIsKinematic(true);

		AddComponent<CircleCollider2D>()
			.SetRadius(0.1f)
			.SetTrigger(true);

		AddComponent<StateMachine>()
			.AddState(E_StateType::BlackHole_Move, new BlackHoleState_Move)
			.AddState(E_StateType::BlackHole_Expand, new BlackHoleState_Expand)
			.AddState(E_StateType::BlackHole_Gravity, new BlackHoleState_Gravity)
			.SetDefaultState(E_StateType::BlackHole_Move);

		AddComponent<Rotator>()
			.SetRotateSpeed(250.0f)
			.enabled = false;

		AddComponent<Gravity>()
			.SetGravity(100.0f)
			.AddAffectedTag(E_Tag::Enemy)
			.AddAffectedTag(E_Tag::UI)
			.SetRange(6.0f)
			.enabled = false;

		AddComponent<Mover>()
			.SetMoveSpeed(4.0f)
			.SetDistance(10.0f)
			.SetMoveDir(GetLstickVec())
			.enabled = false;

		AddComponent<BlackHoleBehaviour>();
	}
};