#include "Sprite.h"
#include "D3DMgr.h"
#include "Texture2D.h"

// Spriteクラスのコンストラクタ（基本的な初期化）
Sprite::Sprite(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& res)
    : vertexBuffer(D3DMgr::instance.GetDevice(), D3DMgr::instance.GetDeviceContext())
{
    texture2D = std::make_shared<Texture2D>(res);  // テクスチャをラップするTexture2Dを作成
    if (texture2D)
    {
        // テクスチャの幅と高さをピクセル単位からワールド単位に変換
        width = static_cast<float>(texture2D->GetWidth()) / pixelsPerUnit;
        height = static_cast<float>(texture2D->GetHeight()) / pixelsPerUnit;
    }

    // スクエアのメッシュを作成（UVマッピングも指定）
    mesh = Geometry::CreateSquareMesh(XMFLOAT2(width, height), uv);

    // 頂点バッファを初期化
    vertexBuffer.Init(mesh);
}

// Spriteクラスの別のコンストラクタ（スプライトシート対応）
Sprite::Sprite(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& res, UVRect uv, XMFLOAT2 spritesPerSize)
    : vertexBuffer(D3DMgr::instance.GetDevice(), D3DMgr::instance.GetDeviceContext())
{
    texture2D = std::make_shared<Texture2D>(res);  // テクスチャリソースをTexture2Dにラップ
    if (texture2D)
    {
        // スプライトごとのサイズをワールド単位に変換（スプライトシートの一部を切り出すため）
        width = spritesPerSize.x / pixelsPerUnit;
        height = spritesPerSize.y / pixelsPerUnit;
    }

    this->uv = uv;  // 使用するUV領域を保存
    mesh = Geometry::CreateSquareMesh(XMFLOAT2(width, height), uv);  // メッシュ作成
    vertexBuffer.Init(mesh);  // 頂点バッファ初期化
}

// UVを取得（左右反転オプション付き）
UVRect Sprite::GetUV(bool isFlipped) const
{
    UVRect result = uv;
    if (isFlipped)
        std::swap(result.u0, result.u1);  // 左右反転：UVの左端と右端を入れ替える
    return result;
}

float Sprite::GetHeight() const 
{
    return height;
}

float Sprite::GetWidth() const 
{
    return width;
}

XMFLOAT2 Sprite::GetSize() const
{
    return { width, height };
}

std::shared_ptr<Texture2D> Sprite::GetTexture2D() const
{
    return texture2D;
}

VertexBuffer& Sprite::GetVertexBuffer() 
{
    return vertexBuffer;
}

float Sprite::GetPixelsPerUnit() const 
{
    return pixelsPerUnit;
}

const Mesh& Sprite::GetMesh() const 
{
    return mesh;
}

// UVを設定し直して、メッシュと頂点バッファを再構築
void Sprite::SetUV(const UVRect& newUV)
{
    uv = newUV;
    mesh = Geometry::CreateSquareMesh(XMFLOAT2(width, height), uv);  // 新しいUVでメッシュ再生成
    vertexBuffer.Init(mesh);  // 頂点バッファ再初期化
}
