#include "GameTime.h"
#include "WinMin.h"

double Time::secondsPerCount = 0.0;
double Time::deltaTime = -1.0;
double Time::unscaledDeltaTime = -1.0;
double Time::scaledTime = 0.0;

float Time::timeScale = 1.0f;
float Time::fixedDeltaTime = 0.02f;

__int64 Time::baseTime = 0;
__int64 Time::prevTime = 0;
__int64 Time::currTime = 0;

float Time::GetTime()
{
	return static_cast<float>(scaledTime);
}

float Time::GetUnscaledTime()
{
	return static_cast<float>((currTime - baseTime) * secondsPerCount);
}

float Time::GetDeltaTime()
{
	return static_cast<float>(deltaTime);
}

float Time::GetUnscaledDeltaTime()
{
	return static_cast<float>(unscaledDeltaTime);
}

float Time::GetTimeScale()
{
	return timeScale;
}

void Time::SetTimeScale(float val)
{
	timeScale = val;
}

float Time::GetFixedDeltaTime()
{
	return fixedDeltaTime;
}

void Time::Reset()
{
	__int64 countsPerSec{};
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	secondsPerCount = 1.0 / (double)countsPerSec;

	__int64 currTime{};
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	baseTime = currTime;
	prevTime = currTime;
	currTime = currTime;
	scaledTime = 0.0;
	unscaledDeltaTime = -1.0;
	deltaTime = -1.0;
}

void Time::Tick()
{
	__int64 currTime{};
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	currTime = currTime;

	unscaledDeltaTime = (currTime - prevTime) * secondsPerCount;
	if (unscaledDeltaTime < 0.0)
		unscaledDeltaTime = 0.0;

	deltaTime = unscaledDeltaTime * timeScale;
	scaledTime += deltaTime;

	prevTime = currTime;
}