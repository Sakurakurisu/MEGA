#include "Animator.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "EventCenter.h"
#include "GameTime.h"

void Animator::Start()
{
	spriteRenderer = gameObject->GetComponent<SpriteRenderer>(); // アニメーション表示用のスプライト取得
}

void Animator::Update()
{
	if (!playing || !currentAnim) return;

	timeCounter += Time::GetDeltaTime(); // 経過時間を加算

	// 指定されたフレーム時間を超えたら次のフレームへ
	if (timeCounter >= animTimePerFrame / playSpeed)
	{
		timeCounter = 0.0f;
		idxFrame++; // フレームインデックスを進める

		if (idxFrame >= animFrameCount)
		{
			if (animIsLoop)
				idxFrame = 0; // ループ再生
			else
			{
				idxFrame = animFrameCount - 1; // 最終フレームで停止
				playing = false;

				// アニメーション完了イベントを発火
				EventCenter::instance.EventTrigger<GameObject*>(E_EventType::Anim_Complete, gameObject);
			}
		}

		// フレームごとのイベントがあるかチェックして実行
		auto typeIt = animFrameEvents.find(currentAnimType);
		if (typeIt != animFrameEvents.end())
		{
			auto frameIt = typeIt->second.find(idxFrame);
			if (frameIt != typeIt->second.end())
			{
				for (const auto& event : frameIt->second)
				{
					if (event)
						event(); // 登録されたイベント関数を実行
				}
			}
		}
	}

	// スプライトの切り替え（実際の描画用画像を更新）
	if (spriteRenderer && currentAnim)
	{
		spriteRenderer->SetSprite(currentAnim->GetFrameSprite(idxFrame));
	}
}


void Animator::PlayAnimation(E_AnimType type)
{
	auto it = anims.find(type);
	if (it != anims.end())
	{
		currentAnim = it->second.get();
		ResetCurrentAnim();      // 状態初期化
		playing = true;
		currentAnimType = type;

		// 最初のスプライトを描画
		if (spriteRenderer && currentAnim)
			spriteRenderer->SetSprite(currentAnim->GetFrameSprite(idxFrame));

		// アニメーション情報の初期化
		animTimePerFrame = currentAnim->GetTimePerFrame();
		animFrameCount = currentAnim->GetFrameCount();
		animIsLoop = currentAnim->IsLoop();
	}
	else
		throw std::runtime_error("Animation not found"); // 指定アニメーションが存在しない場合
}

void Animator::StopAnimation()
{
	playing = false;
	ResetCurrentAnim(); // 状態リセット
}

Animator& Animator::AddAnim(E_AnimType type, std::shared_ptr<AnimationClip> anim)
{
	anims.emplace(type, std::move(anim)); // アニメーションクリップを登録

	return *this;
}

Animator& Animator::SetPlaySpeed(float speed)
{
	playSpeed = speed; // 再生速度を設定

	return *this;
}

float Animator::GetPlaySpeed() const
{
	return playSpeed;
}

bool Animator::IsPlaying() const
{
	return playing;
}

Animator& Animator::AddFrameEvent(E_AnimType type, int frame, FrameEvent event)
{
	animFrameEvents[type][frame].emplace_back(std::move(event)); // 指定フレームにイベントを登録

	return *this;
}

void Animator::ResetCurrentAnim()
{
	// アニメーション再生の状態を初期化
	timeCounter = 0.0f;
	idxFrame = 0;

	animTimePerFrame = 0.0f;
	animFrameCount = 0;
	animIsLoop = false;
}