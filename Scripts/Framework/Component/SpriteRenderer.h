#pragma once

#include <DirectXMath.h>
#include "Component.h"
#include "Atlas.h"
#include "VertexBuffer.h"
#include "Camera.h"
#include "Sprite.h"
#include "Effects.h"

using namespace DirectX;
using namespace Microsoft::WRL;

struct RenderStateConfig;

class SpriteRenderer : public Component
{
public:
   SpriteRenderer() = default;

   // X軸方向に反転を設定します
   SpriteRenderer& SetFlippedX(bool flag);

   // Y軸方向に反転を設定します
   SpriteRenderer& SetFlippedY(bool flag);

   // Z軸方向に反転を設定します
   SpriteRenderer& SetFlippedZ(bool flag);

   // スプライトを設定します
   SpriteRenderer& SetSprite(std::shared_ptr<Sprite> newSprite);

   // アルファ値を設定します
   SpriteRenderer& SetAlpha(float alpha);

   // 色を設定します
   SpriteRenderer& SetColor(const XMFLOAT4& newColor);

   // マテリアルを設定します
   SpriteRenderer& SetMaterial(const Material& newMaterial);

   // レンダーステート設定を設定します
   SpriteRenderer& SetRenderStateConfig(std::shared_ptr<RenderStateConfig> config);

   // スプライトを取得します
   std::shared_ptr<Sprite> GetSprite() const;

   // 色を取得します
   XMFLOAT4 GetColor() const;

   // アルファ値を取得します
   float GetAlpha() const;

   // マテリアルを取得します
   Material& GetMaterial();

   // レンダーステート設定を取得します
   std::shared_ptr<RenderStateConfig> GetRenderStateConfig() const;

   void LateUpdate() override;

private:
   void CollectRenderData();
   std::shared_ptr<Sprite> sprite = nullptr;
   XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

   bool isFlippedX = false;
   bool isFlippedY = false;
   bool isFlippedZ = false;

   Material material{};

   std::shared_ptr<RenderStateConfig> renderStateConfig = nullptr;
};
