#pragma once

struct UVRect
{
	float u0, v0;
	float u1, v1;

	UVRect(float u0 = 0.0f, float v0 = 0.0f, float u1 = 1.0f, float v1 = 1.0f)
		: u0(u0), v0(v0), u1(u1), v1(v1) {}
};
