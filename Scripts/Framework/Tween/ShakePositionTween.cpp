#include "ShakePositionTween.h"
#include <DirectXMath.h>
#include <cmath>

ShakePositionTween::ShakePositionTween(
    std::function<DirectX::XMFLOAT3()> getter,
    std::function<void(const DirectX::XMFLOAT3&)> setter,
    float duration,
    float strength,
    int vibrato,
    bool fadeOut)
    : m_Getter(std::move(getter))
    , m_Setter(std::move(setter))
    , m_Strength(strength)
    , m_Vibrato(vibrato)
    , m_FadeOut(fadeOut)
    , m_RandEngine(std::random_device{}())
    , m_Distribution(-1.0f, 1.0f)
    , m_LastOffset({ 0, 0, 0 })
{
    SetDuration(duration);

    m_InitialPos = m_Getter();
}

void ShakePositionTween::OnUpdate(float easedProgress)
{

    DirectX::XMFLOAT3 basePos = m_Getter();

  
    basePos.x -= m_LastOffset.x;
    basePos.y -= m_LastOffset.y;
    basePos.z -= m_LastOffset.z;

    float attenuation = m_FadeOut ? (1.0f - easedProgress) : 1.0f;
    float amplitude = m_Strength * attenuation;

    float offsetX = m_Distribution(m_RandEngine) * amplitude;
    float offsetY = m_Distribution(m_RandEngine) * amplitude;
    DirectX::XMFLOAT3 newOffset = { offsetX, offsetY, 0 };

    DirectX::XMFLOAT3 newPos;
    newPos.x = basePos.x + newOffset.x;
    newPos.y = basePos.y + newOffset.y;
    newPos.z = basePos.z;

    m_LastOffset = newOffset;

    if (m_Setter)
    {
        m_Setter(newPos);
    }
}
