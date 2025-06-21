#pragma once

class Time
{
public:
	static float GetTime();

	static float GetUnscaledTime();

	static float GetDeltaTime();

	static float GetUnscaledDeltaTime();

	static float GetTimeScale();
	static void SetTimeScale(float val);

	static float GetFixedDeltaTime();

	static void Reset();
	static void Tick();

private:
	static double secondsPerCount;
	static double deltaTime;
	static double unscaledDeltaTime;
	static double scaledTime;

	static float timeScale;
	static float fixedDeltaTime;

	static __int64 baseTime;
	static __int64 prevTime;
	static __int64 currTime;
};
