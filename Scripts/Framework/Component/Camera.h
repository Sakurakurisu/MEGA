#pragma once

#include "Component.h"
#include "main.h"
#include "Effects.h"

using namespace DirectX;

class GameObject;

class Camera : public Component
{
public:
Camera() = default;

// カメラの更新処理を行います
void Update() override;

// 正射影行列を設定します
Camera& SetOrthographic(float width, float height, float nearZ, float farZ);

// カメラの幅と高さを設定します
Camera& SetSize(float width, float height);

// カメラの近クリップ面を設定します
Camera& SetNearZ(float nearZ);

// カメラの遠クリップ面を設定します
Camera& SetFarZ(float farZ);

// ビュー行列を取得します
const XMMATRIX& GetViewMatrix() const;

// 射影行列を取得します
const XMMATRIX& GetProjectionMatrix() const;

// 1ユニットあたりのピクセル数を取得します
float GetPixelsPerUnit() const;

// ビュー射影行列を適用します
void ApplyViewProjection() const;

private:
void UpdateViewMatrix();
void UpdateProjectionMatrix();

float width = SCREEN_WIDTH;
float height = SCREEN_HEIGHT;
float nearZ = 0.0f;
float farZ = 10.0f;
XMMATRIX viewMatrix = XMMatrixIdentity();
XMMATRIX projectionMatrix = XMMatrixIdentity();

float pixelsPerUnit = 100.0f;
};
