#include "ShakePositionTween.h"
#include <DirectXMath.h>
#include <cmath>

// コンストラクターの実装
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
    // 合計の持続時間を設定（Tween 基底クラスに SetDuration メソッドがあると仮定）
    SetDuration(duration);

    // 初期位置を記録
    m_InitialPos = m_Getter();
}

// 毎フレームの揺れ処理の更新
void ShakePositionTween::OnUpdate(float easedProgress)
{
    // 1. 現在の物理システムによって計算された実際の位置を取得
    DirectX::XMFLOAT3 basePos = m_Getter();

    // 2. 前回のオフセットを差し引き、実際の物理位置に戻す
    basePos.x -= m_LastOffset.x;
    basePos.y -= m_LastOffset.y;
    basePos.z -= m_LastOffset.z;

    // 3. 現在の進行度に基づいて減衰係数と振幅を計算
    float attenuation = m_FadeOut ? (1.0f - easedProgress) : 1.0f;
    float amplitude = m_Strength * attenuation;

    // 4. 新しいオフセットをランダムに生成（X軸とY軸のみ、Z軸はそのまま）
    float offsetX = m_Distribution(m_RandEngine) * amplitude;
    float offsetY = m_Distribution(m_RandEngine) * amplitude;
    DirectX::XMFLOAT3 newOffset = { offsetX, offsetY, 0 };

    // 5. 基準位置にオフセットを加算し、最終的な位置を算出
    DirectX::XMFLOAT3 newPos;
    newPos.x = basePos.x + newOffset.x;
    newPos.y = basePos.y + newOffset.y;
    newPos.z = basePos.z; // Z軸は変化なし

    // 6. 新しいオフセットを保存し、次のフレームで前回分を打ち消すために使用
    m_LastOffset = newOffset;

    // 7. 新しい位置を適用
    if (m_Setter)
    {
        m_Setter(newPos);
    }
}
