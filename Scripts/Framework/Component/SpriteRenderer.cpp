#include "SpriteRenderer.h"  
#include "D3DMgr.h"  
#include "Camera.h"  
#include "GameObject.h"  
#include "RenderSystem.h"  
#include "Texture2D.h"  
#include <format>  

SpriteRenderer& SpriteRenderer::SetFlippedX(bool flag)
{
   auto scale = gameObject->transform.localScale;  
   scale.x = std::abs(scale.x) * (flag ? -1.0f : 1.0f);  
   gameObject->transform.localScale = scale;  
   return *this;  
}  

SpriteRenderer& SpriteRenderer::SetFlippedY(bool flag)
{
   auto scale = gameObject->transform.localScale;  
   scale.y = std::abs(scale.y) * (flag ? -1.0f : 1.0f);  
   gameObject->transform.localScale = scale;  
   return *this;  
}  

SpriteRenderer& SpriteRenderer::SetFlippedZ(bool flag)
{
   auto scale = gameObject->transform.localScale;  
   scale.z = std::abs(scale.z) * (flag ? -1.0f : 1.0f);  
   gameObject->transform.localScale = scale;  
   return *this;  
}  

SpriteRenderer& SpriteRenderer::SetSprite(std::shared_ptr<Sprite> newSprite)
{
   sprite = newSprite;  

   return *this;  
}  

SpriteRenderer& SpriteRenderer::SetAlpha(float alpha)
{
   color.w = alpha;  

   return *this;  
}  

SpriteRenderer& SpriteRenderer::SetColor(const XMFLOAT4& newColor)
{
   color = newColor;  

   return *this;  
}  

SpriteRenderer& SpriteRenderer::SetMaterial(const Material& newMaterial)
{
   material = newMaterial;  

   return *this;  
}  

SpriteRenderer& SpriteRenderer::SetRenderStateConfig(std::shared_ptr<RenderStateConfig> config)
{
   renderStateConfig = config;  

   return *this;  
}  

std::shared_ptr<Sprite> SpriteRenderer::GetSprite() const
{
   return sprite;
}

XMFLOAT4 SpriteRenderer::GetColor() const
{
   return color;
}

float SpriteRenderer::GetAlpha() const
{
   return color.w;
}

Material& SpriteRenderer::GetMaterial()
{
   return material;
}

std::shared_ptr<RenderStateConfig> SpriteRenderer::GetRenderStateConfig() const
{
   return renderStateConfig;
}  

void SpriteRenderer::LateUpdate()  
{  
   // ゲームオブジェクトが破棄予定でない場合、レンダーデータを収集  
   if (!gameObject->IsPendingDestroy())  
       CollectRenderData();  
}  

void SpriteRenderer::CollectRenderData()  
{  
   // スプライトが存在しない場合は処理を終了  
   if (!sprite)  
       return;  

   RenderCommand command;  

   // レンダーコマンドの設定  
   command.gameObject = gameObject;  
   command.texture = sprite->GetTexture2D()->GetShaderResourceView().Get();  
   command.worldMatrix = gameObject->transform.GetWorldMatrix();  
   command.color = color;  
   command.material = material;  
   command.mesh = sprite->GetMesh();  

   command.vertexBuffer = &sprite->GetVertexBuffer();  
   command.renderStateConfig = renderStateConfig.get();  

   // レンダーシステムにコマンドを追加  
   RenderSystem::instance.AddRenderCommand(command);  
}