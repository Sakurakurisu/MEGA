#pragma once

#include<cmath>

inline float Repeat(float angle, float length)
{
	return std::fmod(std::fmod(angle, length) + length, length);
}
