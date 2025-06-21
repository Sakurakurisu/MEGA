#include "TimerMgr.h"

// TimerMgr クラスのコンストラクタ
TimerMgr::TimerMgr() : nextTimerId(1)
{}

// 一定時間後にコールバックを実行するタイマーを設定
TimerMgr::TimerId TimerMgr::SetTimeout(float waitTime, Callback callback, bool useScaledTime)
{
Timer timer{
	nextTimerId++, // タイマーIDをインクリメント
	waitTime,      // 待機時間
	std::move(callback), // コールバック関数
	useScaledTime, // スケールされた時間を使用するか
	Time::GetUnscaledTime(), // 現在の非スケール時間
	Time::GetTime(),         // 現在のスケール時間
	false                    // 繰り返しフラグ
};
timers.push_back(std::move(timer)); // タイマーをリストに追加
return timer.id; // タイマーIDを返す
}

// 一定間隔でコールバックを繰り返し実行するタイマーを設定
TimerMgr::TimerId TimerMgr::SetInterval(float interval, Callback callback, bool useScaledTime)
{
Timer timer{
	nextTimerId++, // タイマーIDをインクリメント
	interval,      // インターバル時間
	std::move(callback), // コールバック関数
	useScaledTime, // スケールされた時間を使用するか
	Time::GetUnscaledTime(), // 現在の非スケール時間
	Time::GetTime(),         // 現在のスケール時間
	true                     // 繰り返しフラグ
};
timers.push_back(std::move(timer)); // タイマーをリストに追加
return timer.id; // タイマーIDを返す
}

// 指定したIDのタイマーをキャンセル
void TimerMgr::CancelTimer(TimerId id)
{
std::erase_if(timers, [id](const Timer& timer)
     {
      return timer.id == id; // IDが一致するタイマーを削除
     });
}

// タイマーの状態を更新
void TimerMgr::Update()
{
float currentTime = Time::GetUnscaledTime(); // 現在の非スケール時間
float scaledTime = Time::GetTime();         // 現在のスケール時間

for (auto it = timers.begin(); it != timers.end();)
{
	Timer& timer = *it;
	float elapsedTime = timer.useScaledTime
		? scaledTime - timer.startScaledTime // スケール時間を使用
		: currentTime - timer.startTime;    // 非スケール時間を使用

	if (elapsedTime >= timer.interval) // 指定時間が経過した場合
	{
		timer.callback(); // コールバックを実行

		if (timer.isRepeating) // 繰り返しタイマーの場合
		{
			if (timer.useScaledTime)
				timer.startScaledTime = scaledTime; // スケール時間を更新
			else
				timer.startTime = currentTime;      // 非スケール時間を更新
			++it;
		}
		else
			it = timers.erase(it); // 繰り返しでない場合は削除
	}
	else
		++it;
}
}

// 全てのタイマーをクリア
void TimerMgr::ClearAllTimer()
{
timers.clear(); // タイマーリストをクリア
}
