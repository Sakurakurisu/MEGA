#pragma once
#include <functional>
#include <random>
#include <DirectXMath.h>
#include "Tween.h"  // Tween 基底クラスが正しく定義されていることを確認してください

class ShakePositionTween : public Tween
{
public:
    // コンストラクター：Getter/Setter を通じてオブジェクトの位置を取得・設定
    ShakePositionTween(
        std::function<DirectX::XMFLOAT3()> getter,
        std::function<void(const DirectX::XMFLOAT3&)> setter,
        float duration,
        float strength,
        int vibrato,
        bool fadeOut);

    virtual ~ShakePositionTween() = default;

protected:
    // 各フレーム更新時に呼び出され、easedProgress に基づいてオフセットを更新
    virtual void OnUpdate(float easedProgress) override;

private:
    std::function<DirectX::XMFLOAT3()> m_Getter;                  // 現在の位置（物理効果を含む）を取得する関数
    std::function<void(const DirectX::XMFLOAT3&)> m_Setter;       // 位置を設定する関数

    DirectX::XMFLOAT3 m_InitialPos;   // 初期位置の記録
    float m_Strength;                 // 揺れの強さ
    int m_Vibrato;                    // 揺れの頻度パラメータ（用途に応じて使用）
    bool m_FadeOut;                   // 時間経過によって減衰させるかどうか

    // 乱数生成関連
    std::default_random_engine m_RandEngine;
    std::uniform_real_distribution<float> m_Distribution;

    // 前回の揺れオフセットを記録、次のフレームで元に戻すために使用
    DirectX::XMFLOAT3 m_LastOffset;
};
