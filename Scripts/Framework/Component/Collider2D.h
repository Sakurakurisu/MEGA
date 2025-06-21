#pragma once

#include "Component.h"
#include <DirectXMath.h>
#include "VertexBuffer.h"

using namespace DirectX;

enum class E_ColliderType
{
Box2D,
Circle2D,
};

class Collider2D : public Component
{
public:

// 位置を取得します。
XMFLOAT2 GetPosition() const;

// トリガーかどうかを確認します。
bool IsTrigger() const;

VertexBuffer debugVertexBuffer;
Mesh debugMesh;

// コライダーの種類を取得します。
virtual E_ColliderType GetColliderType() const = 0;

// デバッグ描画を行います。
void DrawDebug(XMFLOAT4 color);

void OnEnable() override;

void OnDisable() override;

void OnDestroy() override;

protected:
Collider2D();
virtual ~Collider2D() = default;
XMFLOAT2 offset = XMFLOAT2(0.0f, 0.0f);
bool isTrigger = false;
};

class BoxCollider2D : public Collider2D
{
public:
BoxCollider2D() = default;

void Init() override;

void Start() override;

// サイズを取得します。
XMFLOAT2 GetSize() const;

// サイズを設定します。
BoxCollider2D& SetSize(float width, float height);

// オフセットを設定します。
BoxCollider2D& SetOffset(float x, float y);

// トリガーを有効または無効にします。
BoxCollider2D& SetTrigger(bool enabled);

// コライダーの種類を取得します。
E_ColliderType GetColliderType() const override;

private:
XMFLOAT2 size = XMFLOAT2(1.0f, 1.0f);
bool isSetSize = false;
};

class CircleCollider2D : public Collider2D
{
public:
CircleCollider2D() = default;

void Init() override;

void Start() override;

// 半径を取得します。
float GetRadius() const;

// 半径を設定します。
CircleCollider2D& SetRadius(float ratio);

// オフセットを設定します。
CircleCollider2D& SetOffset(float x, float y);

// トリガーを有効または無効にします。
CircleCollider2D& SetTrigger(bool enabled);

// コライダーの種類を取得します。
E_ColliderType GetColliderType() const override;

private:
float radius = 0.5f;
bool isSetRadius = false;
};
