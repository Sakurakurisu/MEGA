#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include "Tween.h"
#include "BaseManager.h"

class TweenManager : public BaseManager<TweenManager>
{
	friend class BaseManager<TweenManager>;
public:

	void AddTween(std::shared_ptr<Tween> tween);

	bool RemoveTween(const std::shared_ptr<Tween>& tween);

	void Update();

	void KillAll(bool complete = false);

	void PauseAll();

	void ResumeAll();

private:
	TweenManager() = default;
	TweenManager(const TweenManager&) = delete;
	TweenManager& operator=(const TweenManager&) = delete;

	std::vector<std::shared_ptr<Tween>> m_Tweens;
};
