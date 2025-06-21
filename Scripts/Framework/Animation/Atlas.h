#pragma once  

#include "Sprite.h"  

class Atlas  
{  
public:  
   // コンストラクタ: テクスチャリソース、列数、行数を指定してAtlasを作成します。  
   Atlas(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& res, unsigned columns, unsigned rows);  

   // スプライトの総数を取得します。  
   int GetSpriteCount() const;  

   // 指定されたインデックスのスプライトを取得します。  
   std::shared_ptr<Sprite> GetSprite(unsigned int index) const;  

   // テクスチャ2Dオブジェクトを取得します。  
   std::shared_ptr<Texture2D> GetTexture2D() const;  

private:  
   void GenerateFrameUVs(const UINT textureWidth, const UINT textureHeight, const float spritesPerWidth, const float spritesPerHeight, const unsigned rows, const unsigned columns);  

   std::shared_ptr<Texture2D> texture2D;  
   std::vector<UVRect> uvs;  
   std::vector<std::shared_ptr<Sprite>> sprites;  
   float spritesPerWidth = 0.0f;  
   float spritesPerHeight = 0.0f;  
};
