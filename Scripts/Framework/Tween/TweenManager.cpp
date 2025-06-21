#include "TweenManager.h"
#include <algorithm>

void TweenManager::AddTween(std::shared_ptr<Tween> tween)
{
	if (std::find(m_Tweens.begin(), m_Tweens.end(), tween) != m_Tweens.end()) return;

	m_Tweens.push_back(std::move(tween));
}

void TweenManager::Update()
{
    for (size_t i = 0; i < m_Tweens.size(); ++i)
    {
        if (!m_Tweens[i]->IsFinished())
        {
            m_Tweens[i]->Update();
        }
    }

    std::erase_if(m_Tweens, [](const std::shared_ptr<Tween>& tw)
                  {
                      return tw->IsFinished();
                  });
}

void TweenManager::KillAll(bool complete) 
{
    for (size_t i = 0; i < m_Tweens.size(); ++i)
    {
        m_Tweens[i]->Kill(complete);
    }
}

void TweenManager::PauseAll()
{
    for (size_t i = 0; i < m_Tweens.size(); ++i)
    {
        m_Tweens[i]->Pause();
    }
}

void TweenManager::ResumeAll()
{
    for (size_t i = 0; i < m_Tweens.size(); ++i)
    {
        m_Tweens[i]->Resume();
    }
}

bool TweenManager::RemoveTween(const std::shared_ptr<Tween>& tween)
{
	auto it = std::find(m_Tweens.begin(), m_Tweens.end(), tween);
	if (it != m_Tweens.end())
	{
		m_Tweens.erase(it);
		return true;
	}
	return false;
}