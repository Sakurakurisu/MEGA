#pragma once

#include <vector>
#include <memory>
#include <cmath>
#include <algorithm>
#include "Tween.h"
#include "TweenManager.h"

// シーケンス内の1つのTweenとその時間情報
struct SequenceItem
{
	std::shared_ptr<Tween> tween;       // 対象のTween
	float startTime = 0.0f;             // シーケンス内での開始時刻
	float effectiveDuration = 0.0f;     // 遅延・ループを考慮した実行時間
};

// 複数のTweenを順に（または並列で）実行するためのシーケンス管理クラス
class Sequence : public Tween
{
public:
	Sequence()
		: Tween()
		, m_SequenceDuration(0.0f)
	{}

	// Tweenを末尾に追加（順次実行）
	Sequence* Append(std::shared_ptr<Tween> child) 
	{
		TweenManager::instance.RemoveTween(child); // Tween管理から一時除外

		float childEffDur = CalculateChildEffectiveDuration(child);
		float itemStart = m_SequenceDuration;

		SequenceItem item;
		item.tween = child;
		item.startTime = itemStart;
		item.effectiveDuration = childEffDur;

		m_Items.push_back(item);
		m_SequenceDuration += childEffDur;
		
		SetDuration(m_SequenceDuration);

		return this;
	}

	// Tweenを現在の末尾と同時に開始（並列実行）
	Sequence* Join(std::shared_ptr<Tween> child)
	
	{

		TweenManager::instance.RemoveTween(child);

		float childEffDur = CalculateChildEffectiveDuration(child);

		float joinStart = 0.0f;
		if (!m_Items.empty())
		{
			joinStart = m_Items.back().startTime;
		}

		SequenceItem item;
		item.tween = child;
		item.startTime = joinStart;
		item.effectiveDuration = childEffDur;

		m_Items.push_back(item);

		float endTime = joinStart + childEffDur;
		if (endTime > m_SequenceDuration)
		{
			m_SequenceDuration = endTime;
			SetDuration(m_SequenceDuration);
		}
		return this;
	}

	// シーケンス全体の進行に基づいて、各Tweenに適切な進捗を割り当てる
	void OnUpdate(float normalizedProgress) override
	{
		float seqTime = normalizedProgress * m_SequenceDuration;
		for (auto& item : m_Items)
		{
			auto tw = item.tween;
			if (!tw) continue;

			float localTime = seqTime - item.startTime;

			if (localTime <= 0.0f)
			{
				ForceTweenProgress(tw, 0.0f);
			}
			else if (localTime >= item.effectiveDuration)
			{
				ForceTweenProgress(tw, 1.0f);
			}
			else
			{
				float subProgress = CalculateSubProgress(tw, localTime);
				ForceTweenProgress(tw, subProgress);
			}
		}
	}

private:
	// 遅延やループを含めたTweenの実行全体時間を算出
	float CalculateChildEffectiveDuration(const std::shared_ptr<Tween>& tw)
	{
		float baseDur = tw->m_Duration;
		float delay = tw->m_Delay;
		int   loops = tw->m_Loops;
		if (loops < 0)
		{
			loops = 1;
		}
		float singleLoopLen = baseDur;
		if (tw->m_LoopType == LoopType::Yoyo)
		{
			singleLoopLen = baseDur * 2.0f;
		}

		float total = delay + singleLoopLen * loops;
		return total;
	}

	// 単一Tween内での進捗率を算出（Yoyo対応）
	float CalculateSubProgress(const std::shared_ptr<Tween>& tw, float localTime) 
	{
		float delay = tw->m_Delay;
		float baseDur = tw->m_Duration;
		int   loops = tw->m_Loops;
		LoopType lt = tw->m_LoopType;

		if (localTime < delay)
		{
			return 0.0f;
		}
		float playTime = localTime - delay;

		float singleLen = (lt == LoopType::Yoyo) ? (baseDur * 2.0f) : baseDur;

		int loopIndex = (int)std::floor(playTime / singleLen);
		if (loopIndex >= loops)
		{
			return 1.0f;
		}

		float timeInLoop = playTime - (loopIndex * singleLen);

		if (lt == LoopType::Restart)
		{
			return (std::min)(timeInLoop / baseDur, 1.0f);
		}
		else
		{
			// Yoyo: 再帰フェーズの進行計算
			if (timeInLoop <= baseDur)
			{
				return (std::min)(timeInLoop / baseDur, 1.0f);
			}
			else
			{
				float back = timeInLoop - baseDur;
				float ratio = back / baseDur;  // 0~1
				float reversed = 1.0f - ratio;

				return (std::max)(reversed, 0.0f);
			}
		}
	}

	// 指定された進捗でTweenを強制更新
	void ForceTweenProgress(const std::shared_ptr<Tween>& tw, float p) 
	{
		
		tw->OnUpdate(p);
	}

private:
	std::vector<SequenceItem> m_Items;  // 登録されたTween一覧
	float m_SequenceDuration;           // シーケンス全体の長さ（秒）
};
