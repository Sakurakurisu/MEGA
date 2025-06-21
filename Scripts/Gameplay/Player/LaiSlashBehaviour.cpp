#include "LaiSlashBehaviour.h"
#include "GameObject.h"
#include "GameTime.h"
#include "StateMachine.h"
#include "E_StateType.h"
#include "DOTween.h"
#include "EventCenter.h"
#include "PlayerBehaviour.h"
#include "Input.h"
#include "AttackInfo.h"
#include "Camera.h"
#include "SpriteRenderer.h"
#include "Mathf.h"
#include <Debug.h>
#include "Health.h"
#include "TimerMgr.h"
#include "SoundMgr.h"

void LaiSlashBehaviour::Start()
{
	// ブラックホール生成/破壊イベントを監視
	EventCenter::instance.AddEventListener(E_EventType::Player_BlackHoleCreate, this, &LaiSlashBehaviour::OnBlackHoleCreated);
	EventCenter::instance.AddEventListener(E_EventType::BlackHole_Destroy, this, &LaiSlashBehaviour::OnBlackHoleDestroy);

	rb = gameObject->GetComponent<Rigidbody2D>();
	playerBehaviour = gameObject->GetComponent<PlayerBehaviour>();

	laiSlashArrow = GameObject::Find("lasiSlashArrow");
}

void LaiSlashBehaviour::UpdateLaiSlash()
{
	chargeSystem.UpdateChargeTime(Time::GetUnscaledDeltaTime());

	// ブラックホールが範囲内に存在するかチェック
	bool blackHoleInRange = blackHoleEntity && Distance(gameObject->transform.position, blackHoleEntity->transform.position) < checkDistance;
	if (blackHoleInRange)
	{
		XMFLOAT2 blackHoleDir = ToXMFLOAT2(Direction(gameObject->transform.position, blackHoleEntity->transform.position));
		XMFLOAT2 playerDir = GetLeftJoystick();

		// プレイヤー入力方向との角度が許容範囲内か確認
		isAiming = (Angle(blackHoleDir, playerDir) < checkAngle);
	}
	else
	{
		isAiming = false;
	}
}

void LaiSlashBehaviour::UpdateArrow()
{
	if (!laiSlashArrow) return;

	if (isAiming)
	{
		ChangeArrowEnable(true);
		float angle = SignedAngle(XMFLOAT2(1, 0), ToXMFLOAT2(Direction(gameObject->transform.position, blackHoleEntity->transform.position)));

		// 矢印の向きをブラックホール方向へ回転
		if (laiSlashArrow->transform.GetParent()->scale.x < 0.0f)
			angle = 180.0f - angle;

		Repeat(angle, 360.0f);
		laiSlashArrow->transform.localRotation = angle;
	}
	else
		ChangeArrowEnable(false);
}

void LaiSlashBehaviour::OnDestroy()
{
	EventCenter::instance.RemoveEventListener(E_EventType::Player_BlackHoleCreate, this, &LaiSlashBehaviour::OnBlackHoleCreated);
	EventCenter::instance.RemoveEventListener(E_EventType::BlackHole_Destroy, this, &LaiSlashBehaviour::OnBlackHoleDestroy);
}

LaiSlashBehaviour& LaiSlashBehaviour::SetMinDistance(float dis)
{
	minDistance = dis;

	return *this;
}

LaiSlashBehaviour& LaiSlashBehaviour::SetMaxDistance(float dis)
{
	maxDistance = dis;

	return *this;
}

LaiSlashBehaviour& LaiSlashBehaviour::SetChargeTime(float minTime, float maxTime)
{
	chargeSystem.SetChargeTime(minTime, maxTime);

	return *this;
}

LaiSlashBehaviour& LaiSlashBehaviour::SetDirection(const XMFLOAT2& dir)
{
	direction = dir;

	return *this;
}

LaiSlashBehaviour& LaiSlashBehaviour::SetCheckDistance(float dis)
{
	checkDistance = dis;

	return *this;
}

LaiSlashBehaviour& LaiSlashBehaviour::SetCheckAngle(float angle)
{
	checkAngle = angle;

	return *this;
}

LaiSlashBehaviour& LaiSlashBehaviour::SetDashDistance(float dis)
{
	dashDistance = dis;
	return *this;
}

void LaiSlashBehaviour::DoLaiSlash()
{
	if (!rb || !playerBehaviour) return;

	isCompleted = false;
	ChangeArrowEnable(false);

	// 無敵状態を一時的に付与
	gameObject->GetComponent<Health>()->SetActiveInvincivle(1.0f);

	// ヒット検知イベント登録
	EventCenter::instance.AddEventListener(E_EventType::Attack, this, &LaiSlashBehaviour::OnHit);

	// 瞄準状態ならブラックホール方向、そうでなければ入力方向
	direction = isAiming ?
		ToXMFLOAT2(Direction(gameObject->transform.position, blackHoleEntity->transform.position)) :
		GetLaiSlashDirection();

	// 向きを設定
	if (direction.x > 0)
		playerBehaviour->SetFacingDirection(E_FacingDirection::Right);
	else if (direction.x < 0)
		playerBehaviour->SetFacingDirection(E_FacingDirection::Left);

	// チャージ段階に応じて距離を決定
	if (chargeSystem.IsMinCharged())
		finalDistance = minDistance;
	else if (chargeSystem.IsNotFullyCharged())
		finalDistance = chargeSystem.GetChargeRatio() * maxDistance;
	else
		finalDistance = maxDistance;

	XMFLOAT3 targetPos = gameObject->transform.position + direction * finalDistance;

	// 既存Tween停止
	chargeTween->Kill();
	flashTween->Kill();

	// スラッシュ移動Tween開始
	laiSlashTween = DOTween::DOMove(rb, targetPos, 0.5f)
		->SetEase(EaseType::ExpOut)
		->OnPlay([this]()
				 {
					 auto vec = rb->GetVelocity();
					 rb->SetVelocity({ vec.x, 0.0f });
					 rb->SetGravityScale(0.0f);
				 })
		->OnComplete([this]()
					 {
						 EventCenter::instance.RemoveEventListener(E_EventType::Attack, this, &LaiSlashBehaviour::OnHit);
						 isCompleted = true;
						 laiSlashTween = nullptr;
					 })
		->OnKill([this]()
				 {
					 laiSlashTween = nullptr;
				 });
}

void LaiSlashBehaviour::StartCharge()
{
	chargeSystem.StartCharge();

	// フラッシュ & シェイク演出開始
	flashTween = DOTween::DOColor(gameObject->GetComponent<SpriteRenderer>()->GetMaterial(), XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f), 0.05f)
		->OnKill([this]()
				 {
					 gameObject->GetComponent<SpriteRenderer>()->GetMaterial().diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
				 })
		->SetLoops(-1, LoopType::Yoyo);
	chargeTween = DOTween::DOShakeLocalPosition(gameObject->transform, 1.5f, 0.04f, 10, true);
}

bool LaiSlashBehaviour::IsCompleted() const
{
	return isCompleted;
}

bool LaiSlashBehaviour::ShouldJump() const
{
	return shouldJump;
}

XMFLOAT2 LaiSlashBehaviour::GetLaiSlashDirection() const
{
	XMFLOAT2 inputVelocity = GetLeftJoystick();

	if (inputVelocity.x > 0)
		playerBehaviour->SetFacingDirection(E_FacingDirection::Right);
	else if (inputVelocity.x < 0)
		playerBehaviour->SetFacingDirection(E_FacingDirection::Left);

	return (playerBehaviour->GetFacingDirection() == E_FacingDirection::Right) ? XMFLOAT2{ 1.0f, 0.0f } : XMFLOAT2{ -1.0f, 0.0f };
}

void LaiSlashBehaviour::OnBlackHoleCreated(GameObject* obj)
{
	blackHoleEntity = obj;
}

void LaiSlashBehaviour::OnBlackHoleDestroy()
{
	blackHoleEntity = nullptr;
}

void LaiSlashBehaviour::OnHit(AttackInfo attackInfo)
{
	if (attackInfo.target->tag != E_Tag::Enemy) return;

	shouldJump = true;

	// ダメージ処理 + ヒット効果
	attackInfo.target->GetComponent<Health>()->BeAttacked(999);
	SoundMgr::instance.CreateAudioClip("Hit01")->Play();

	if (laiSlashTween)
	{
		laiSlashTween->Kill();

		// カメラシェイク → 二段ダッシュ演出
		Camera* mainCamera = gameObject->scene->GetMainCamera();
		DOTween::DOShakePosition(mainCamera->gameObject->transform, 0.2f, 2.0f, 10, true)
			->SetEase(EaseType::QuadOut)
			->OnComplete([this]()
						 {
							 DOTween::DOMove(rb, gameObject->transform.position + direction * dashDistance, 0.2f)
								 ->SetEase(EaseType::QuadOut)
								 ->OnPlay([this]()
										  {
											  auto vec = rb->GetVelocity();
											  rb->SetVelocity({ vec.x,0.0f });
											  rb->SetGravityScale(0.0f);
										  })
								 ->OnComplete([this]()
											  {
												  TimerMgr::instance.SetTimeout(0.2f, [this]()
																				{
																				    EventCenter::instance.RemoveEventListener(E_EventType::Attack, this, &LaiSlashBehaviour::OnHit);

																					isCompleted = true;
																					laiSlashTween = nullptr;
																				});
											  });
						 });
	}
}
void LaiSlashBehaviour::ChangeArrowEnable(bool flag)
{
	if (!laiSlashArrow) return;

	laiSlashArrow->GetComponent<SpriteRenderer>()->enabled = flag;
}

void LaiSlashBehaviour::Reset()
{
	isAiming = false;
	shouldJump = false;
	chargeSystem.Reset();
}