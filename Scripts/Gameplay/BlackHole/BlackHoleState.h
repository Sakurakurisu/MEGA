#pragma once

#include "BaseState.h"
#include "Rotator.h"
#include "Input.h"
#include "Gravity.h"
#include "Mover.h"
#include "EventCenter.h"
#include "DoTween.h"
#include "AudioClip.h"
#include "SoundMgr.h"
#include "BlackHoleBehaviour.h"
#include "Debug.h"

// ブラックホール移動状態
class BlackHoleState_Move : public BaseState
{
public:
	void Enter() override
	{
		// 移動コンポーネントを有効化
		gameObject->GetComponent<Mover>()->enabled = true;
	}

	void Update() override
	{
		// 特定ボタン入力で拡大状態へ遷移
		if (IsButtonDown(ButtonKind::Button_West) || IsButtonDown(ButtonKind::Button_North) || IsButtonDown(ButtonKind::Button_R1))
		{
			stateMachine->ChangeState(E_StateType::BlackHole_Expand);
		}
	}

	void Exit() override
	{
		// 移動無効化 & 音再生
		gameObject->GetComponent<Mover>()->enabled = false;
		gameObject->GetComponent<BlackHoleBehaviour>()->gravitySound->Play(-1);
	}
};

// ブラックホール拡大状態
class BlackHoleState_Expand : public BaseState
{
public:

	float currentGrayMix = 1.0f;

	void Enter() override
	{
		// 回転・重力有効化、アニメ再生
		gameObject->GetComponent<Rotator>()->enabled = true;
		gameObject->GetComponent<Gravity>()->enabled = true;
		gameObject->GetComponent<Animator>()->PlayAnimation(E_AnimType::BlackHole_Expand);

		// ブラックホール拡大アニメーション
		DOTween::DOScale(gameObject->transform, XMFLOAT3(4.0f, 4.0f, 1.0f), 0.1f)
			->SetEase(EaseType::QuadInOut);

		// イベント通知（他のシステムとの連携）
		EventCenter::instance.EventTrigger(E_EventType::Player_BlackHoleExpand);

		// 画面グレースケール化（GrayMix補間）
		DOTween::To<float>(
			[this]() -> float
			{
				return currentGrayMix;
			},
			[this](const float& val)
			{
				currentGrayMix = val;
			},
			0.0f,
			0.3f)
			->OnUpdateValue([this](const float& newVal)
							{
								BasicEffect::instance.SetGrayMix(newVal);
							})
			->OnComplete([this]()
						 {
							 currentGrayMix = 1.0f;  // 初期値に戻す
						 })
			->SetEase(EaseType::QuadOut)
			->Play();
	}

	void Update() override
	{
		// 吸引状態へ遷移
		if (IsButtonPush(ButtonKind::Button_R1))
			stateMachine->ChangeState(E_StateType::BlackHole_Gravity);

		// ブラックホールをキャンセル（縮小 & 破棄）
		if (IsButtonDown(Button_West))
		{
			gameObject->GetComponent<BlackHoleBehaviour>()->gravitySound->Stop();

			DOTween::DOScale(gameObject->transform, XMFLOAT3(0.0f, 0.0f, 1.0f), 0.1f)
				->SetEase(EaseType::QuadInOut)
				->OnComplete([this]()
							 {
								 stateMachine->Destroy(stateMachine->gameObject);
							 });
		}
	}
};

// ブラックホール吸引状態
class BlackHoleState_Gravity : public BaseState
{
public:
	void Enter() override
	{
		// 重力影響対象にプレイヤーを追加
		gameObject->GetComponent<Gravity>()->AddAffectedTag(E_Tag::Player);
	}

	void Update() override
	{
		// ボタン解除で拡大状態に戻る
		if (!IsButtonPush(ButtonKind::Button_R1))
			stateMachine->ChangeState(E_StateType::BlackHole_Expand);

		// 中断入力でブラックホールを破棄
		if (IsButtonDown(Button_West))
			stateMachine->Destroy(stateMachine->gameObject);
	}

	void Exit() override
	{
		// プレイヤーへの影響解除
		gameObject->GetComponent<Gravity>()->RemoveAffectedTag(E_Tag::Player);
	}
};
