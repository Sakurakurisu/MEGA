#pragma once

#include "VertexBuffer.h"
#include <memory>

class Texture2D;

class Sprite
{
public:
   // コンストラクタ: シェーダーリソースビューを使用してスプライトを初期化します。
   Sprite(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& res);
   // コンストラクタ: UV座標とサイズを指定してスプライトを初期化します。
   Sprite(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& res, UVRect uv, XMFLOAT2 size);

   // UV座標を取得します。フリップ状態に応じて異なるUVを返します。
   UVRect GetUV(bool isFlipped) const;
   // スプライトの高さを取得します。
   float GetHeight() const;
   // スプライトの幅を取得します。
   float GetWidth() const;
   // スプライトのサイズを取得します。
   XMFLOAT2 GetSize() const;
   // スプライトに関連付けられたTexture2Dを取得します。
   std::shared_ptr<Texture2D> GetTexture2D() const;
   // スプライトの頂点バッファーを取得します。
   VertexBuffer& GetVertexBuffer();
   // ピクセル単位あたりのユニット数を取得します。
   float GetPixelsPerUnit() const;
   // スプライトのメッシュを取得します。
   const Mesh& GetMesh() const;

   // UV座標を設定します。
   void SetUV(const UVRect& newUV);

private:
   std::shared_ptr<Texture2D> texture2D;
   UVRect uv = UVRect(0.0f, 0.0f, 1.0f, 1.0f);
   float width = 0.0f;
   float height = 0.0f;
   float pixelsPerUnit = 100.0f;

   Mesh mesh;
   VertexBuffer vertexBuffer;
};
