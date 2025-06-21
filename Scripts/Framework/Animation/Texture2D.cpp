#include "Texture2D.h"
#include <stdexcept>

// コンストラクタ：ShaderResourceViewからTexture2Dを初期化
Texture2D::Texture2D(const ComPtr<ID3D11ShaderResourceView>& srv) : textureSRV(srv)
{
    // 無効なSRVポインタが渡された場合、例外を投げる
    if (!textureSRV)
    {
        throw std::runtime_error("Invalid ShaderResourceView pointer provided.");
    }

    ComPtr<ID3D11Resource> resource;
    // ShaderResourceViewから基になるリソースを取得
    textureSRV->GetResource(resource.GetAddressOf());

    ComPtr<ID3D11Texture2D> texture2D;
    // リソースが2Dテクスチャであるかを確認し、キャストする
    if (SUCCEEDED(resource.As(&texture2D)))
    {
        D3D11_TEXTURE2D_DESC desc;
        texture2D->GetDesc(&desc);  // テクスチャの詳細情報（幅や高さなど）を取得

        // 幅と高さを保存（これらは後でスプライトのサイズ決定に使われる）
        width = desc.Width;
        height = desc.Height;
    }
}

const ComPtr<ID3D11ShaderResourceView>& Texture2D::GetShaderResourceView() const
{
    return textureSRV;
}

UINT Texture2D::GetWidth() const
{
    return width;
}

UINT Texture2D::GetHeight() const
{
    return height;
}
