#include "SpriteRenderer.h"  
#include "D3DMgr.h"  
#include "Camera.h"  
#include "GameObject.h"  
#include "RenderSystem.h"  
#include "Texture2D.h"  
#include <format>  

SpriteRenderer& SpriteRenderer::SetFlippedX(bool flag)  
{  
   // X軸方向の反転を設定  
   auto scale = gameObject->transform.localScale;  
   scale.x = std::abs(scale.x) * (flag ? -1.0f : 1.0f);  
   gameObject->transform.localScale = scale;  
   return *this;  
}  

SpriteRenderer& SpriteRenderer::SetFlippedY(bool flag)  
{  
   // Y軸方向の反転を設定  
   auto scale = gameObject->transform.localScale;  
   scale.y = std::abs(scale.y) * (flag ? -1.0f : 1.0f);  
   gameObject->transform.localScale = scale;  
   return *this;  
}  

SpriteRenderer& SpriteRenderer::SetFlippedZ(bool flag)  
{  
   // Z軸方向の反転を設定  
   auto scale = gameObject->transform.localScale;  
   scale.z = std::abs(scale.z) * (flag ? -1.0f : 1.0f);  
   gameObject->transform.localScale = scale;  
   return *this;  
}  

SpriteRenderer& SpriteRenderer::SetSprite(std::shared_ptr<Sprite> newSprite)  
{  
   // スプライトを設定  
   sprite = newSprite;  

   return *this;  
}  

SpriteRenderer& SpriteRenderer::SetAlpha(float alpha)  
{  
   // アルファ値を設定  
   color.w = alpha;  

   return *this;  
}  

SpriteRenderer& SpriteRenderer::SetColor(const XMFLOAT4& newColor)  
{  
   // 色を設定  
   color = newColor;  

   return *this;  
}  

SpriteRenderer& SpriteRenderer::SetMaterial(const Material& newMaterial)  
{  
   // マテリアルを設定  
   material = newMaterial;  

   return *this;  
}  

SpriteRenderer& SpriteRenderer::SetRenderStateConfig(std::shared_ptr<RenderStateConfig> config)  
{  
   // レンダーステート設定を設定  
   renderStateConfig = config;  

   return *this;  
}  

std::shared_ptr<Sprite> SpriteRenderer::GetSprite() const  
{  
   // スプライトを取得  
   return sprite;  
}  

XMFLOAT4 SpriteRenderer::GetColor() const  
{  
   // 色を取得  
   return color;  
}  

float SpriteRenderer::GetAlpha() const  
{  
   // アルファ値を取得  
   return color.w;  
}  

Material& SpriteRenderer::GetMaterial()  
{  
   // マテリアルを取得  
   return material;  
}  

std::shared_ptr<RenderStateConfig> SpriteRenderer::GetRenderStateConfig() const  
{  
   // レンダーステート設定を取得  
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