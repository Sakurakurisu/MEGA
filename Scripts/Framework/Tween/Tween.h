#pragma once
#include <functional>
#include "Ease.h"
#include "LoopType.h"
#include "GameTime.h"

class Sequence;

enum class TweenState
{
	Idle,
	Playing,
	Paused,
	Completed,
	Killed
};

class Tween
{
	friend class Sequence;

public:
	Tween()
		: m_Duration(1.0f)
		, m_Elapsed(0.0f)
		, m_Delay(0.0f)
		, m_CurrentDelay(0.0f)
		, m_State(TweenState::Idle)
		, m_EaseType(EaseType::Linear)
		, m_Reverse(false)
		, m_LoopType(LoopType::Restart)
		, m_Loops(1)    
		, m_CompletedLoops(0)
		, m_Started(false)
	{}

	virtual ~Tween() = default;

	void Update() {
		if (m_State != TweenState::Playing) return;

		if (m_CurrentDelay > 0.0f)
		{
			m_CurrentDelay -= Time::GetDeltaTime();
			if (m_CurrentDelay <= 0.0f)
			{
				m_CurrentDelay = 0.0f;
				if (!m_Started)
				{
					m_Started = true;
					if (m_OnPlay) m_OnPlay();
				}
			}
			return;
		}

		if (!m_Started)
		{
			m_Started = true;
			if (m_OnPlay) m_OnPlay();
		}

		float scaledDt = Time::GetDeltaTime();

		m_Elapsed += (m_Reverse ? -scaledDt : scaledDt);

		if (m_Elapsed > m_Duration)
		{
			m_Elapsed = m_Duration;
		}
		else if (m_Elapsed < 0.0f)
		{
			m_Elapsed = 0.0f;
		}

		float progress = m_Elapsed / m_Duration;
		float eased = EvaluateEase(progress);

		OnUpdate(eased);

		if (m_Elapsed >= m_Duration || m_Elapsed <= 0.0f)
		{
			m_CompletedLoops++;
			if (m_Loops < 0 || m_CompletedLoops < m_Loops)
			{
				if (m_OnStepComplete) m_OnStepComplete();

				switch (m_LoopType)
				{
					case LoopType::Restart:
					{
						m_Elapsed = (m_Reverse ? m_Duration : 0.0f);
					} break;

					case LoopType::Yoyo:
					{
						m_Reverse = !m_Reverse;
					}break;
				}
			}
			else
			{
				m_State = TweenState::Completed;
				OnUpdate(m_Reverse ? 0.0f : 1.0f);
				if (m_OnComplete)
					m_OnComplete();
			}
		}
	}

	virtual Tween* SetDuration(float duration) {
		m_Duration = (duration <= 0.0f) ? 0.0001f : duration;
		return this;
	}

	virtual Tween* SetDelay(float delay) {
		m_Delay = delay;
		m_CurrentDelay = delay;
		return this;
	}

	virtual Tween* SetEase(EaseType ease) {
		m_EaseType = ease;
		return this;
	}

	virtual Tween* SetLoops(int loops, LoopType loopType = LoopType::Restart) {
		m_Loops = loops;
		m_LoopType = loopType;
		return this;
	}

	virtual Tween* OnPlay(std::function<void()> callback) {
		m_OnPlay = std::move(callback);
		return this;
	}

	virtual Tween* OnPause(std::function<void()> callback) {
		m_OnPause = std::move(callback);
		return this;
	}

	virtual Tween* OnStepComplete(std::function<void()> callback) {
		m_OnStepComplete = std::move(callback);
		return this;
	}

	virtual Tween* OnComplete(std::function<void()> callback) {
		m_OnComplete = std::move(callback);
		return this;
	}

	virtual Tween* OnKill(std::function<void()> callback) {
		m_OnKill = std::move(callback);
		return this;
	}

	virtual Tween* Play() {
		if (m_State == TweenState::Paused)
		{
			m_State = TweenState::Playing;
			if (m_OnPlay) m_OnPlay();
			return this;
		}

		m_State = TweenState::Playing;
		return this;
	}

	virtual Tween* Pause() {
		if (m_State == TweenState::Playing)
		{
			m_State = TweenState::Paused;
			if (m_OnPause) m_OnPause();
		}
		return this;
	}

	virtual Tween* Resume() {
		if (m_State == TweenState::Paused)
		{
			m_State = TweenState::Playing;
			if (m_OnPlay) m_OnPlay();
		}
		return this;
	}

	virtual Tween* Restart() {
		m_Elapsed = 0.0f;
		m_CurrentDelay = m_Delay;
		m_Reverse = false;
		m_CompletedLoops = 0;
		m_State = TweenState::Idle;
		m_Started = false;
		return this;
	}

	virtual Tween* Kill(bool complete = false)
	{
		if (complete)
		{
			m_State = TweenState::Completed;
	
			OnUpdate(m_Reverse ? 0.0f : 1.0f);
			if (m_OnComplete)
				m_OnComplete();
		}
		else
		{
			m_State = TweenState::Killed;
			if (m_OnKill)
				m_OnKill();
		}
		return this;
	}

	bool IsFinished() const {
		return (m_State == TweenState::Completed || m_State == TweenState::Killed);
	}

	TweenState GetState() const {
		return m_State;
	}

protected:

	virtual void OnUpdate(float easedProgress) = 0;

	float EvaluateEase(float t) {
		return Evaluate(m_EaseType, t);
	}

protected:
	float m_Duration;
	float m_Elapsed;

	float m_Delay;
	float m_CurrentDelay;

	TweenState m_State;
	EaseType   m_EaseType;

	bool m_Reverse;

	LoopType m_LoopType;
	int      m_Loops; // <0 => infinite
	int      m_CompletedLoops;

	bool  m_Started;

	std::function<void()> m_OnPlay;
	std::function<void()> m_OnPause;
	std::function<void()> m_OnStepComplete;
	std::function<void()> m_OnComplete;
	std::function<void()> m_OnKill;
};
