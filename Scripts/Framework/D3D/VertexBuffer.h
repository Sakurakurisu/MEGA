#pragma once

#include <wrl.h>
#include <vector>
#include "Geometry.h"

using Microsoft::WRL::ComPtr;

class VertexBuffer
{
public:
// コンストラクタ: デバイスとコンテキストを初期化します。
VertexBuffer(ID3D11Device* device, ID3D11DeviceContext* context);

// 初期化: メッシュデータを使用してバッファを初期化します。
void Init(Mesh& meshData);

// バインド: バッファをバインドして描画準備を行います。
void Bind() const;

// 描画: バインドされたバッファを使用して描画を実行します。
void Draw() const;

// 頂点更新: メッシュデータを使用して頂点バッファを更新します。
void UpdateVertices(const Mesh& mesh) const;

private:
ComPtr<ID3D11Buffer> vertexBuffer = nullptr;
ComPtr<ID3D11Buffer> indexBuffer = nullptr;

ID3D11Device* device;
ID3D11DeviceContext* context;

UINT vertexStride = 0;
UINT indexCount = 0;
UINT vertexCount = 0;
};
