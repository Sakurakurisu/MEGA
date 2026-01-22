#pragma once

#include <cmath>

constexpr float M_PI = 3.141592654f;

enum class EaseType
{
	Linear,
	QuadIn,
	QuadOut,
	QuadInOut,
	CubicIn,
	CubicOut,
	CubicInOut,
	QuartIn,
	QuartOut,
	QuartInOut,
	QuintIn,
	QuintOut,
	QuintInOut,
	SineIn,
	SineOut,
	SineInOut,
	ExpIn,
	ExpOut,
	ExpInOut,
	CircIn,
	CircOut,
	CircInOut,
	ElasticIn,
	ElasticOut,
	ElasticInOut,
	BackIn,
	BackOut,
	BackInOut,
	BounceIn,
	BounceOut,
	BounceInOut
};

inline float EaseLinear(float t)
{
	return t;
}

inline float EaseQuadIn(float t)
{
	return t * t;
}

inline float EaseQuadOut(float t)
{
	return 1.0f - (1.0f - t) * (1.0f - t);
}

inline float EaseQuadInOut(float t)
{
	if (t < 0.5f)
		return 2.0f * t * t;

	return 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
}

inline float EaseCubicIn(float t)
{
	return t * t * t;
}

inline float EaseCubicOut(float t)
{
	float p = 1.0f - t;
	return 1.0f - p * p * p;
}

inline float EaseCubicInOut(float t)
{
	if (t < 0.5f)
		return 4.0f * t * t * t;

	float f = (2.0f * t - 2.0f);
	return 0.5f * f * f * f + 1.0f;
}
inline float EaseQuartIn(float t)
{
	return t * t * t * t;
}

inline float EaseQuartOut(float t)
{
	float p = 1.0f - t;
	return 1.0f - p * p * p * p;
}

inline float EaseQuartInOut(float t)
{
	if (t < 0.5f)
		return 8.0f * t * t * t * t;
	float p = -2.0f * t + 2.0f;
	return 1.0f - p * p * p * p / 2.0f;
}

// Quint関数
inline float EaseQuintIn(float t)
{
	return t * t * t * t * t;
}

inline float EaseQuintOut(float t)
{
	float p = 1.0f - t;
	return 1.0f - p * p * p * p * p;
}

inline float EaseQuintInOut(float t)
{
	if (t < 0.5f)
		return 16.0f * t * t * t * t * t;
	float p = -2.0f * t + 2.0f;
	return 1.0f - p * p * p * p * p / 2.0f;
}

// Sine関数
inline float EaseSineIn(float t)
{
	return 1.0f - std::cos(t * M_PI / 2.0f);
}

inline float EaseSineOut(float t)
{
	return std::sin(t * M_PI / 2.0f);
}

inline float EaseSineInOut(float t)
{
	return -(std::cos(M_PI * t) - 1.0f) / 2.0f;
}

// Exponential関数
inline float EaseExpIn(float t)
{
	return t == 0.0f ? 0.0f : std::pow(2.0f, 10.0f * t - 10.0f);
}

inline float EaseExpOut(float t)
{
	return t == 1.0f ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * t);
}

inline float EaseExpInOut(float t)
{
	if (t == 0.0f) return 0.0f;
	if (t == 1.0f) return 1.0f;
	if (t < 0.5f)
		return std::pow(2.0f, 20.0f * t - 10.0f) / 2.0f;
	return (2.0f - std::pow(2.0f, -20.0f * t + 10.0f)) / 2.0f;
}

// Circular関数
inline float EaseCircIn(float t)
{
	return 1.0f - std::sqrt(1.0f - t * t);
}

inline float EaseCircOut(float t)
{
	return std::sqrt(1.0f - std::pow(t - 1.0f, 2.0f));
}

inline float EaseCircInOut(float t)
{
	if (t < 0.5f)
		return (1.0f - std::sqrt(1.0f - std::pow(2.0f * t, 2.0f))) / 2.0f;
	return (std::sqrt(1.0f - std::pow(-2.0f * t + 2.0f, 2.0f)) + 1.0f) / 2.0f;
}

// Elastic関数
inline float EaseElasticIn(float t)
{
	const float c4 = (2.0f * M_PI) / 3.0f;
	if (t == 0.0f) return 0.0f;
	if (t == 1.0f) return 1.0f;
	return -std::pow(2.0f, 10.0f * t - 10.0f) * std::sin((t * 10.0f - 10.75f) * c4);
}

inline float EaseElasticOut(float t)
{
	const float c4 = (2.0f * M_PI) / 3.0f;
	if (t == 0.0f) return 0.0f;
	if (t == 1.0f) return 1.0f;
	return std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * c4) + 1.0f;
}

inline float EaseElasticInOut(float t)
{
	const float c5 = (2.0f * M_PI) / 4.5f;
	if (t == 0.0f) return 0.0f;
	if (t == 1.0f) return 1.0f;
	if (t < 0.5f)
		return -(std::pow(2.0f, 20.0f * t - 10.0f) * std::sin((20.0f * t - 11.125f) * c5)) / 2.0f;
	return (std::pow(2.0f, -20.0f * t + 10.0f) * std::sin((20.0f * t - 11.125f) * c5)) / 2.0f + 1.0f;
}

// Back関数
inline float EaseBackIn(float t)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;
	return c3 * t * t * t - c1 * t * t;
}

inline float EaseBackOut(float t)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;
	float p = t - 1.0f;
	return 1.0f + c3 * p * p * p + c1 * p * p;
}

inline float EaseBackInOut(float t)
{
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;
	if (t < 0.5f)
		return (std::pow(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2)) / 2.0f;
	float p = 2.0f * t - 2.0f;
	return (std::pow(p, 2.0f) * ((c2 + 1.0f) * p + c2) + 2.0f) / 2.0f;
}

// Bounce関数
inline float EaseBounceOut(float t)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (t < 1.0f / d1)
	{
		return n1 * t * t;
	}
	else if (t < 2.0f / d1)
	{
		t -= 1.5f / d1;
		return n1 * t * t + 0.75f;
	}
	else if (t < 2.5f / d1)
	{
		t -= 2.25f / d1;
		return n1 * t * t + 0.9375f;
	}
	else
	{
		t -= 2.625f / d1;
		return n1 * t * t + 0.984375f;
	}
}

inline float EaseBounceIn(float t)
{
	return 1.0f - EaseBounceOut(1.0f - t);
}

inline float EaseBounceInOut(float t)
{
	if (t < 0.5f)
		return (1.0f - EaseBounceOut(1.0f - 2.0f * t)) / 2.0f;
	return (1.0f + EaseBounceOut(2.0f * t - 1.0f)) / 2.0f;
}

inline float Evaluate(EaseType type, float t)
{
	switch (type)
	{
		case EaseType::Linear:        return EaseLinear(t);
		case EaseType::QuadIn:        return EaseQuadIn(t);
		case EaseType::QuadOut:       return EaseQuadOut(t);
		case EaseType::QuadInOut:     return EaseQuadInOut(t);
		case EaseType::CubicIn:       return EaseCubicIn(t);
		case EaseType::CubicOut:      return EaseCubicOut(t);
		case EaseType::CubicInOut:    return EaseCubicInOut(t);
		case EaseType::QuartIn:       return EaseQuartIn(t);
		case EaseType::QuartOut:      return EaseQuartOut(t);
		case EaseType::QuartInOut:    return EaseQuartInOut(t);
		case EaseType::QuintIn:       return EaseQuintIn(t);
		case EaseType::QuintOut:      return EaseQuintOut(t);
		case EaseType::QuintInOut:    return EaseQuintInOut(t);
		case EaseType::SineIn:        return EaseSineIn(t);
		case EaseType::SineOut:       return EaseSineOut(t);
		case EaseType::SineInOut:     return EaseSineInOut(t);
		case EaseType::ExpIn:         return EaseExpIn(t);
		case EaseType::ExpOut:        return EaseExpOut(t);
		case EaseType::ExpInOut:      return EaseExpInOut(t);
		case EaseType::CircIn:        return EaseCircIn(t);
		case EaseType::CircOut:       return EaseCircOut(t);
		case EaseType::CircInOut:     return EaseCircInOut(t);
		case EaseType::ElasticIn:     return EaseElasticIn(t);
		case EaseType::ElasticOut:    return EaseElasticOut(t);
		case EaseType::ElasticInOut:  return EaseElasticInOut(t);
		case EaseType::BackIn:        return EaseBackIn(t);
		case EaseType::BackOut:       return EaseBackOut(t);
		case EaseType::BackInOut:     return EaseBackInOut(t);
		case EaseType::BounceIn:      return EaseBounceIn(t);
		case EaseType::BounceOut:     return EaseBounceOut(t);
		case EaseType::BounceInOut:   return EaseBounceInOut(t);
	}
	return t;
}
