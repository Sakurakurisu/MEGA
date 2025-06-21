#pragma once
#include <functional>
#include <random>
#include <DirectXMath.h>
#include "Tween.h" 

class ShakePositionTween : public Tween
{
public:

    ShakePositionTween(
        std::function<DirectX::XMFLOAT3()> getter,
        std::function<void(const DirectX::XMFLOAT3&)> setter,
        float duration,
        float strength,
        int vibrato,
        bool fadeOut);

    virtual ~ShakePositionTween() = default;

protected:

    virtual void OnUpdate(float easedProgress) override;

private:
    std::function<DirectX::XMFLOAT3()> m_Getter;              
    std::function<void(const DirectX::XMFLOAT3&)> m_Setter;   

    DirectX::XMFLOAT3 m_InitialPos;  
    float m_Strength;               
    int m_Vibrato;                  
    bool m_FadeOut;                 

    std::default_random_engine m_RandEngine;
    std::uniform_real_distribution<float> m_Distribution;
    DirectX::XMFLOAT3 m_LastOffset;
};
