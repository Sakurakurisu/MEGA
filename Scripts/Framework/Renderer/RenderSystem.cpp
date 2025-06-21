#include "RenderSystem.h"  
#include "Camera.h"  
#include "D3DMgr.h"  
#include "Debug.h"  
#include "Vector.h"  
#include "GameObject.h"  
#include "TextUtil.h"  
#include <dwrite.h>  
#include "DXTrace.h"  

void RenderSystem::AddRenderCommand(const RenderCommand& command)  
{  
   renderCommands.push_back(command);  
}  

void RenderSystem::AddRenderTextCommand(const TextCommand& command)  
{  
   textCommands.push_back(command);  
}  

void RenderSystem::Render(Camera* camera)  
{  
   ID3D11DeviceContext* context = D3DMgr::instance.GetDeviceContext();  
   if (!context) return;  

   camera->ApplyViewProjection();  

   {  
       auto viewMatrix = camera->GetViewMatrix();  

       // 描画コマンドをZ値でソートする  
       std::sort(renderCommands.begin(), renderCommands.end(),  
                 [viewMatrix](const RenderCommand& a, const RenderCommand& b)  
                 {  
                     XMVECTOR posA = XMVector3TransformCoord(XMVectorSet(0.f, 0.f, 0.f, 1.f), a.worldMatrix * viewMatrix);  
                     XMVECTOR posB = XMVector3TransformCoord(XMVectorSet(0.f, 0.f, 0.f, 1.f), b.worldMatrix * viewMatrix);  

                     float zA = XMVectorGetZ(posA);  
                     float zB = XMVectorGetZ(posB);  

                     return zA < zB;  
                 });  
   }  

   for (const auto& command : renderCommands)  
   {  
       if (command.gameObject->IsPendingDestroy()) continue;  

       if (command.renderStateConfig)  
           BasicEffect::instance.SetRenderState(context, *command.renderStateConfig);  
       else  
           Debug::Log(":Render state is not set!", Debug::LogLevel::WARN);  

       BasicEffect::instance.SetWorldMatrix(command.worldMatrix);  

       command.vertexBuffer->UpdateVertices(command.mesh);  
       command.vertexBuffer->Bind();  

       BasicEffect::instance.SetTexture(command.texture);  
       BasicEffect::instance.SetColor(command.color);  
       BasicEffect::instance.SetMaterial(command.material);  

       BasicEffect::instance.Apply(context);  
       command.vertexBuffer->Draw();  
   }  

   // シェーダーリソースビューをクリアする  
   ID3D11ShaderResourceView* nullView = nullptr;  
   D3DMgr::instance.GetDeviceContext()->PSSetShaderResources(0, 1, &nullView);  

   renderCommands.clear();  
}  

void RenderSystem::RenderText(Camera* camera)  
{  
   ID2D1DeviceContext* context = D3DMgr::instance.GetD2DDeviceContext();  
   context->BeginDraw();  

   const XMMATRIX viewMatrix = camera->GetViewMatrix();  
   const XMMATRIX projMatrix = camera->GetProjectionMatrix();  
   const XMMATRIX worldMatrix = XMMatrixIdentity();  
   const float viewportWidth = (float)SCREEN_WIDTH;  
   const float viewportHeight = (float)SCREEN_HEIGHT;  

   for (const auto& command : textCommands)  
   {  
       std::wstring textStr = TextUtil::ConvertToWideString(command.text);  

       // DirectWriteのテキストフォーマットを作成  
       HR(D3DMgr::instance.dwriteFactory->CreateTextFormat(  
           command.fontFamilyName,  
           nullptr,  
           DWRITE_FONT_WEIGHT_NORMAL,  
           DWRITE_FONT_STYLE_NORMAL,  
           DWRITE_FONT_STRETCH_NORMAL,  
           command.fontSize,  
           L"ja-jp",  
           D3DMgr::instance.textFormat.GetAddressOf()));  

       XMFLOAT2 textMetrics = TextUtil::GetTextSize(textStr, D3DMgr::instance.textFormat.Get());  

       XMVECTOR worldPos = XMVectorSet(command.position.x, command.position.y, command.position.z, 1.0f);  

       // ワールド座標をスクリーン座標に変換  
       XMVECTOR projected = XMVector3Project(  
           worldPos,  
           0.0f,  
           0.0f,  
           viewportWidth,  
           viewportHeight,  
           0.0f,  
           1.0f,  
           projMatrix,  
           viewMatrix,  
           worldMatrix  
       );  

       XMFLOAT3 screenPos{};  
       XMStoreFloat3(&screenPos, projected);  

       float halfWidth = textMetrics.x * 0.5f;  
       float halfHeight = textMetrics.y * 0.5f;  
       screenPos.x = floorf(screenPos.x);  
       screenPos.y = floorf(screenPos.y);  

       D2D1_RECT_F rect = D2D1::RectF(  
           screenPos.x - halfWidth,  
           screenPos.y - halfHeight,  
           screenPos.x + halfWidth,  
           screenPos.y + halfHeight  
       );  

       // テキストを描画  
       context->DrawTextW(  
           textStr.c_str(),  
           (UINT32)textStr.length(),  
           D3DMgr::instance.textFormat.Get(),  
           rect,  
           D3DMgr::instance.colorBrush.Get());  
   }  

   // 描画を終了  
   HR(context->EndDraw());  
   textCommands.clear();  
}