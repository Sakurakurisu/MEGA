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

   SpriteRenderer& SetFlippedX(bool flag);
   SpriteRenderer& SetFlippedY(bool flag);
   SpriteRenderer& SetFlippedZ(bool flag);
   SpriteRenderer& SetSprite(std::shared_ptr<Sprite> newSprite);
   SpriteRenderer& SetAlpha(float alpha);
   SpriteRenderer& SetColor(const XMFLOAT4& newColor);
   SpriteRenderer& SetMaterial(const Material& newMaterial);
   SpriteRenderer& SetRenderStateConfig(std::shared_ptr<RenderStateConfig> config);

   std::shared_ptr<Sprite> GetSprite() const;
   XMFLOAT4 GetColor() const;
   float GetAlpha() const;
   Material& GetMaterial();
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
