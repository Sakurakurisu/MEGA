#include "GravitySkillBehaviour.h"
#include "BlackHole.h"
#include "keyboard.h"
#include "EventCenter.h"
#include "Input.h"
#include "Vector.h"
#include "TimerMgr.h"
#include "ChargeSystem.h"
#include "DoTween.h"
#include "GroundChecker.h"

void GravitySkillBehaviour::Start()
{
	// ブラックホール破壊イベントの登録
	EventCenter::instance.AddEventListener(E_EventType::BlackHole_Destroy, this, &GravitySkillBehaviour::OnBlackHoleDestroy);
	EventCenter::instance.AddEventListener<XMFLOAT3>(E_EventType::BlackHole_Destroy, this, &GravitySkillBehaviour::CheckJustPoint);
	EventCenter::instance.AddEventListener<GameObject*, bool>(E_EventType::Ground_Check, this, &GravitySkillBehaviour::OnGroundCheck);

	// 地面判定・エフェクト取得
	groundChecker = GameObject::Find("GroundChecker");
	chargeEffect = GameObject::Find("ChargeEffect");
}

void GravitySkillBehaviour::OnDestroy()
{
	// イベントの解除
	EventCenter::instance.RemoveEventListener(E_EventType::BlackHole_Destroy, this, &GravitySkillBehaviour::OnBlackHoleDestroy);
	EventCenter::instance.RemoveEventListener<XMFLOAT3>(E_EventType::BlackHole_Destroy, this, &GravitySkillBehaviour::CheckJustPoint);
}

void GravitySkillBehaviour::CreateBlackHole()
{
	// チャージエフェクト終了
	chargeEffect->GetComponent<Animator>()->StopAnimation();
	chargeEffect->GetComponent<SpriteRenderer>()->enabled = false;

	// ブラックホール生成位置を設定
	XMFLOAT3 pos(gameObject->transform.position.x, gameObject->transform.position.y, 1.0f);

	// ブラックホール本体生成
	GameObject* blackHoleEntity = GameObject::Instantiate<BlackHole>(pos, gameObject->transform.rotation);

	auto chargeRatio = chargeSystem.GetChargeRatio();
	auto range = blackHoleEntity->GetComponent<Gravity>()->GetRange();

	// チャージ量に応じて引力範囲を変更
	if (!chargeSystem.IsMinCharged())
	{
		blackHoleEntity->GetComponent<Gravity>()->SetRange(range * chargeRatio);
	}
	else
	{
		blackHoleEntity->GetComponent<Gravity>()->SetRange(range * 0.5f);
	}

	hasBlackHole = true;

	// プレイヤーのスキル発動イベント通知
	EventCenter::instance.EventTrigger(E_EventType::Player_BlackHoleCreate);

	// 画面のグレースケール補間（1→0へ）
	DOTween::To<float>(
		[this]() -> float
		{
			return currentGrayMix;
		},
		[this](const float& val)
		{
			currentGrayMix = val;
		},
		1.0f,
		0.3f)
		->OnUpdateValue([this](const float& newVal)
						{
							BasicEffect::instance.SetGrayMix(newVal);
						})
		->OnComplete([this]()
					 {
						 currentGrayMix = 0.0f;
					 })
		->SetEase(EaseType::QuadOut)
		->Play();

	// ブラックホール本体もイベント通知に含める
	EventCenter::instance.EventTrigger(E_EventType::Player_BlackHoleCreate, blackHoleEntity);
}

void GravitySkillBehaviour::CheckJustPoint(XMFLOAT3 pos)
{
	// ブラックホールが近距離（3.5未満）で発動したか判定
	auto dis = Distance(ToXMFLOAT2(gameObject->transform.position), ToXMFLOAT2(pos));
	if (dis < 3.5f)
	{
		isJustPoint = true;
		// ジャストポイント成功イベント
		EventCenter::instance.EventTrigger(E_EventType::Player_JustPoint);
		EventCenter::instance.EventTrigger<XMFLOAT3>(E_EventType::Player_JustPoint, pos);
	}
}
bool GravitySkillBehaviour::CanCreateBlackHole()
{
	// クール中または既に存在している場合は不可
	return !isCooling && !hasBlackHole;
}

void GravitySkillBehaviour::SetUsedGravity(bool flag)
{
	usedGravity = flag;
}

void GravitySkillBehaviour::StartCharge()
{
	// チャージ開始
	chargeSystem.StartCharge();
	chargeEffect->GetComponent<SpriteRenderer>()->enabled = true;
	chargeEffect->GetComponent<Animator>()->PlayAnimation(E_AnimType::Charge_Effect);
}

void GravitySkillBehaviour::Update()
{
	// チャージ時間更新
	chargeSystem.UpdateChargeTime(Time::GetDeltaTime());

	// 地面にいるかどうかを更新
	isGrounded = groundChecker->GetComponent<GroundChecker>()->IsGrounded();
}

void GravitySkillBehaviour::OnBlackHoleDestroy()
{
	hasBlackHole = false;

	// 地面にいればクールタイムは発生しない
	if (isGrounded) return;
	isCooling = true;

	// ジャストポイント時はクール短縮
	float coolTime = 3.0f;
	if (isJustPoint)
	{
		coolTime = 1.0f;
		isJustPoint = false;
	}

	// 一定時間後にクール解除
	timerID = TimerMgr::instance.SetTimeout(coolTime, [this]()
											{
												isCooling = false;
											});

	usedGravity = false;
}

void GravitySkillBehaviour::OnGroundCheck(GameObject* obj, bool flag)
{
	if (obj != gameObject) return;

	// 地面接地時にクールタイマーをキャンセル
	TimerMgr::instance.CancelTimer(timerID);
	isCooling = false;
}