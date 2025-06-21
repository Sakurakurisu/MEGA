#include "ResourceMgr.h"  
#include "D3DMgr.h"  
#include "Debug.h"  
#include "DXTrace.h"  
#include "AnimationClip.h"  

using namespace DirectX;  

// スプライトをロードする関数  
bool ResourceMgr::LoadSprite(const SpriteResInfo& info)  
{  
   // すでにロードされている場合は何もしない  
   if (spritePool.find(info.id) != spritePool.end())  
       return true;  

   // テクスチャを作成  
   auto textureView = CreateTexture(info.path);  
   if (!textureView)  
       return false;  

   // スプライトを作成し、プールに追加  
   auto sprite = std::make_shared<Sprite>(textureView);  
   spritePool[info.id] = sprite;  
   return true;  
}  

// アトラスをロードする関数  
bool ResourceMgr::LoadAtlas(const AtlasResInfo& info)  
{  
   // すでにロードされている場合は何もしない  
   if (atlasPool.find(info.id) != atlasPool.end()) return true;  

   // テクスチャを作成  
   auto textureView = CreateTexture(info.path);  
   if (!textureView) return false;  

   // アトラスを作成し、プールに追加  
   auto atlas = std::make_shared<Atlas>(textureView, info.columns, info.rows);  
   atlasPool[info.id] = std::move(atlas);  

   return true;  
}  

// アニメーションクリップをロードする関数  
bool ResourceMgr::LoadAnimationClip(const AnimationResInfo& info)  
{  
   // すでにロードされている場合は何もしない  
   if (animPool.find(info.id) != animPool.end()) return true;  

   // テクスチャを作成  
   auto textureView = CreateTexture(info.path);  
   if (!textureView) return false;  

   // アニメーションクリップを作成し、プールに追加  
   auto anim = std::make_shared<AnimationClip>(std::make_shared<Atlas>(textureView, info.columns, info.rows), info.timePerFrame, info.loop);  
   animPool[info.id] = std::move(anim);  

   return true;  
}  

// テクスチャを作成する関数  
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ResourceMgr::CreateTexture(const std::wstring& filename)  
{  
   TexMetadata metadata{};  
   ScratchImage image;  
   // WICファイルからロード  
   HR(LoadFromWICFile(filename.c_str(), WIC_FLAGS_NONE, &metadata, image));  

   ID3D11Device* device = D3DMgr::instance.GetDevice();  
   if (!device) return nullptr;  

   Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView;  
   // シェーダーリソースビューを作成  
   HR(CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), metadata, textureView.GetAddressOf()));  

   return textureView;  
}  

// リソースをロードする関数  
void ResourceMgr::Load()  
{  
   // スプライト情報をロード  
   for (const auto& info : spriteInfos)  
   {  
       if (!LoadSprite(info))  
           Debug::Log("Failed to load sprite: " + info.id, Debug::LogLevel::ERR);  
   }  

   // アトラス情報をロード  
   for (const auto& info : atlasInfos)  
   {  
       if (!LoadAtlas(info))  
           Debug::Log("Failed to load atlas: " + info.id, Debug::LogLevel::ERR);  
   }  

   // アニメーション情報をロード  
   for (const auto& info : animInfos)  
   {  
       if (!LoadAnimationClip(info))  
           Debug::Log("Failed to load animation: " + info.id, Debug::LogLevel::ERR);  
   }  
}  

// スプライトを取得する関数  
std::shared_ptr<Sprite> ResourceMgr::GetSprite(const std::string& id) const  
{  
   auto it = spritePool.find(id);  

   if (it != spritePool.end())  
       return it->second;  

   // スプライトが見つからない場合は例外をスロー  
   throw std::runtime_error(id + ":Sprite not found");  
   return nullptr;  
}  

// アトラスを取得する関数  
std::shared_ptr<Atlas> ResourceMgr::GetAtlas(const std::string& id) const  
{  
   auto it = atlasPool.find(id);  

   if (it != atlasPool.end())  
       return it->second;  

   // アトラスが見つからない場合は例外をスロー  
   throw std::runtime_error(id + ":Atlas not found");  
   return nullptr;  
}  

// アニメーションクリップを取得する関数  
std::shared_ptr<AnimationClip> ResourceMgr::GetAnimationClip(const std::string& id) const  
{  
   auto it = animPool.find(id);  

   if (it != animPool.end())  
       return it->second;  

   // アニメーションクリップが見つからない場合は例外をスロー  
   throw std::runtime_error(id + ":Animation not found");  
   return nullptr;  
}