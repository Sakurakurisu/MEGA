#pragma once  
#include "GameTime.h"  
#include "BaseManager.h"  
#include <functional>  
#include <vector>  
#include <algorithm>  

class TimerMgr : public BaseManager<TimerMgr>  
{  
friend class BaseManager<TimerMgr>;  

public:  
using TimerId = size_t;  
using Callback = std::function<void()>;  

// 指定した時間後に一度だけコールバックを実行します  
TimerId SetTimeout(float waitTime, Callback callback, bool useScaledTime = true);  

// 指定した間隔で繰り返しコールバックを実行します  
TimerId SetInterval(float interval, Callback callback, bool useScaledTime = true);  

// 指定したタイマーをキャンセルします  
void CancelTimer(TimerId id);  

// タイマーの状態を更新します  
void Update();  

// すべてのタイマーをクリアします  
void ClearAllTimer();  

private:  
struct Timer  
{  
	TimerId id;  
	float interval;  
	Callback callback;  
	bool useScaledTime;  
	float startTime;  
	float startScaledTime;  
	bool isRepeating;  
};  

TimerMgr();  
TimerMgr(const TimerMgr&) = delete;  
TimerMgr& operator=(const TimerMgr&) = delete;  

std::vector<Timer> timers;  
TimerId nextTimerId{};  
};