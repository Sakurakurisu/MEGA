#pragma once
#include <functional>
#include "Tween.h"
#include "Vector.h"

template<typename T>
class PropertyTweener : public Tween
{
public:
	PropertyTweener(std::function<T()> getter,
					std::function<void(const T&)> setter,
					const T& toValue)
		: Tween()
		, m_Getter(std::move(getter))
		, m_Setter(std::move(setter))
		, m_To(toValue)
		, m_UseFrom(false)
		, m_Relative(false)
		, m_Initialized(false)
	{}

	PropertyTweener<T>* SetFrom(const T& fromVal)
	{
		m_From = fromVal;
		m_UseFrom = true;
		return this;
	}

	PropertyTweener<T>* SetRelative(bool relative)
	{
		m_Relative = relative;
		return this;
	}

	PropertyTweener<T>* OnUpdateValue(std::function<void(const T&)> callback)
	{
		m_OnUpdateValue = std::move(callback);
		return this;
	}

	PropertyTweener<T>* ChangeTarget(std::function<T()> newGetter,
									 std::function<void(const T&)> newSetter)
	{
		m_Getter = std::move(newGetter);
		m_Setter = std::move(newSetter);
		return this;
	}

	virtual PropertyTweener<T>* SetEase(EaseType ease) override
	{
		Tween::SetEase(ease);
		return this;
	}

	virtual PropertyTweener<T>* SetLoops(int loops, LoopType loopType = LoopType::Restart) override
	{
		Tween::SetLoops(loops, loopType);
		return this;
	}

	virtual PropertyTweener<T>* OnPlay(std::function<void()> callback) override 
	{
		Tween::OnPlay(callback);
		return this;
	}

	virtual PropertyTweener<T>* OnPause(std::function<void()> callback) override
	{
		Tween::OnPause(callback);
		return this;
	}

	virtual PropertyTweener<T>* OnStepComplete(std::function<void()> callback) override
	{
		Tween::OnStepComplete(callback);
		return this;
	}

	virtual PropertyTweener<T>* OnComplete(std::function<void()> callback) override
	{
		Tween::OnComplete(callback);
		return this;
	}

	virtual PropertyTweener<T>* OnKill(std::function<void()> callback) override
	{
		Tween::OnKill(callback);
		return this;
	}

	virtual PropertyTweener<T>* SetDelay(float delay) override
	{
		Tween::SetDelay(delay);
		return this;
	}

	virtual PropertyTweener<T>* Play() override 
	{
		Tween::Play();
		return this;
	}

	virtual PropertyTweener<T>* Kill(bool complete = false) override 
	{
		Tween::Kill(complete);

		return this;
	}

protected:
	void OnUpdate(float easedProgress) override
	{

		if (!m_Initialized)
		{
			if (!m_UseFrom)
			{
				m_From = m_Getter();
			}
			if (m_Relative)
			{
				m_To = m_From + m_To;
			}
			m_Initialized = true;
		}

		T newVal = Lerp(m_From, m_To, easedProgress);

		if (m_Setter)
		{
			m_Setter(newVal);
		}

		if (m_OnUpdateValue)
		{
			m_OnUpdateValue(newVal);
		}
	}

	T Lerp(const T& a, const T& b, float t)
	{
		return a + (b - a) * t;
	}

private:
	std::function<T()>           m_Getter;
	std::function<void(const T&)> m_Setter;

	T m_From;
	T m_To;

	bool m_UseFrom;
	bool m_Relative;
	bool m_Initialized;

	std::function<void(const T&)> m_OnUpdateValue;
};
