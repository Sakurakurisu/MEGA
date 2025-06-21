#include "PlayerBehaviour.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "AttackInfo.h"
#include "EventCenter.h"
#include "DoTween.h"
#include "Vector.h"
#include "Input.h"
#include "Mathf.h"
#include "FloatingText.h"
#include "RigidBody2D.h"

void PlayerBehaviour::Start()
{
	// コンポーネント取得
	spriteRenderer = gameObject->GetComponent<SpriteRenderer>();
	animator = gameObject->GetComponent<Animator>();
	rb = gameObject->GetComponent<Rigidbody2D>();

	// イベント登録：攻撃、ジャストポイント時の演出
	EventCenter::instance.AddEventListener<AttackInfo>(E_EventType::Attack, this, &PlayerBehaviour::OnAttack);
	EventCenter::instance.AddEventListener(E_EventType::Player_JustPoint, this, &PlayerBehaviour::GenerateFloatingText);
	EventCenter::instance.AddEventListener<XMFLOAT3>(E_EventType::Player_JustPoint, this, &PlayerBehaviour::JustPoint);
}

void PlayerBehaviour::OnDestroy()
{
	// イベント解除
	EventCenter::instance.RemoveEventListener<AttackInfo>(E_EventType::Attack, this, &PlayerBehaviour::OnAttack);
	EventCenter::instance.RemoveEventListener(E_EventType::Player_JustPoint, this, &PlayerBehaviour::GenerateFloatingText);
	EventCenter::instance.RemoveEventListener<XMFLOAT3>(E_EventType::Player_JustPoint, this, &PlayerBehaviour::JustPoint);
}

// 向き設定（左右反転）
PlayerBehaviour& PlayerBehaviour::SetFacingDirection(E_FacingDirection dir)
{
	facingDirection = dir;

	switch (facingDirection)
	{
		case E_FacingDirection::Right:
			spriteRenderer->SetFlippedX(false);
			break;
		case E_FacingDirection::Left:
			spriteRenderer->SetFlippedX(true);
			break;
	}

	return *this;
}

E_FacingDirection PlayerBehaviour::GetFacingDirection() const
{
	return facingDirection;
}

// スティック入力から向き角度を算出
float PlayerBehaviour::CalculateTargetAngle(const XMFLOAT2& stickInput)
{
	float angle = SignedAngle(XMFLOAT2(1, 0), stickInput);

	if (gameObject->transform.scale.x < 0)
		angle += 180.0f;

	Repeat(angle, 360.0f); // 範囲を0〜360に正規化

	return angle;
}

// 回転を瞬時に設定
void PlayerBehaviour::RotateToDirection(const XMFLOAT2& stickInput)
{
	gameObject->transform.rotation = CalculateTargetAngle(stickInput);
}

// 回転を補間して追従（速度指定）
void PlayerBehaviour::RotateToDirection(const XMFLOAT2& stickInput, float rotateSpeed)
{
	float targetAngle = CalculateTargetAngle(stickInput);
	float currentAngle = gameObject->transform.rotation;

	// 緩やかにターゲット角へ回転
	gameObject->transform.rotation = LerpAngle(currentAngle, targetAngle, rotateSpeed * Time::GetDeltaTime());
}

// ジャストポイント時の演出用浮き文字生成
void PlayerBehaviour::GenerateFloatingText()
{
	XMFLOAT3 pos = XMFLOAT3(gameObject->transform.position.x, gameObject->transform.position.y - 1.0f, gameObject->transform.position.z);
	GameObject::Instantiate<FloatingText>()
		->SetPosition(pos);
	// 紫のフラッシュ演出
	DOTween::DOColor(gameObject->GetComponent<SpriteRenderer>()->GetMaterial(), { 0.7f,0.0f,0.7f,0.0f }, 0.125)
		->OnComplete([this]()
					 {
						 // 元の透明状態に戻す
						 gameObject->GetComponent<SpriteRenderer>()->GetMaterial().diffuse = { 0.0f,0.0f,0.0f,0.0f };
					 })
		->SetLoops(6, LoopType::Yoyo);  // 点滅6回
}

// ジャストポイント判定による推進処理
void PlayerBehaviour::JustPoint(XMFLOAT3 pos)
{
	XMFLOAT2 dir = Direction(ToXMFLOAT2(gameObject->transform.position), ToXMFLOAT2(pos));

	XMFLOAT2 vec = rb->GetVelocity();
	Normalize(vec);  // 現在速度の方向を取得

	rb->SetVelocity(vec * 3.0f);  // 速度強化
}

// 自身の攻撃が成立したときの処理
void PlayerBehaviour::OnAttack(AttackInfo attackInfo)
{
	if (attackInfo.attacker == gameObject)
	{
		// ヒットストップ演出
		DOTween::DOHitStop(animator, 0.25f, 0.15f);

		// カメラ取得（演出用？）
		Camera* mainCamera = gameObject->scene->GetMainCamera();
	}
}