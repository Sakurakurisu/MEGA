#include "VertexBuffer.h"  
#include <stdexcept>  
#include "DXTrace.h"  

VertexBuffer::VertexBuffer(ID3D11Device* device, ID3D11DeviceContext* context) : device(device), context(context)  
{}  

void VertexBuffer::Init(Mesh& meshData)  
{  
   vertexBuffer.Reset();  
   indexBuffer.Reset();  

   if (device == nullptr) return;  

   vertexCount = static_cast<UINT>(meshData.vertices.size());  
   vertexStride = sizeof(VertexPosColorTex);  

   // 頂点データを準備する  
   std::vector<VertexPosColorTex> fullVertices(vertexCount);  
   for (UINT i = 0; i < vertexCount; ++i)  
   {  
       fullVertices[i].Position = meshData.vertices[i];  
       fullVertices[i].Color = meshData.colors;  // 色データを設定  
       fullVertices[i].TexCoord = meshData.uv[i];  // テクスチャ座標を設定  
   }  

   // 頂点バッファの設定  
   D3D11_BUFFER_DESC vbd{};  
   vbd.Usage = D3D11_USAGE_DYNAMIC;  
   vbd.ByteWidth = vertexCount * vertexStride;  
   vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;  
   vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;  

   D3D11_SUBRESOURCE_DATA InitData{};  
   InitData.pSysMem = fullVertices.data();  
   HR(device->CreateBuffer(&vbd, &InitData, vertexBuffer.GetAddressOf()));  

   indexCount = static_cast<UINT>(meshData.triangles.size());  

   // インデックスバッファの設定  
   D3D11_BUFFER_DESC ibd{};  
   ibd.Usage = D3D11_USAGE_IMMUTABLE;  
   ibd.ByteWidth = indexCount * sizeof(uint32_t);  
   ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;  
   ibd.CPUAccessFlags = 0;  

   InitData.pSysMem = meshData.triangles.data();  
   HR(device->CreateBuffer(&ibd, &InitData, indexBuffer.GetAddressOf()));  
}  

void VertexBuffer::Bind() const  
{  
   UINT strides = vertexStride;  
   UINT offsets = 0;  
   // 頂点バッファをバインド  
   context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &strides, &offsets);  
   if (indexBuffer)  
       context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);  
}  

void VertexBuffer::Draw() const  
{  
   if (indexBuffer)  
       context->DrawIndexed(indexCount, 0, 0);  // インデックス付き描画  
   else  
       context->Draw(vertexCount, 0);  // 頂点のみの描画  
}  

void VertexBuffer::UpdateVertices(const Mesh& mesh) const  
{  
   if (!vertexBuffer)  
   {  
       throw std::runtime_error("Vertex buffer not initialized!");  
   }  

   if (mesh.vertices.size() != vertexCount)  
   {  
       throw std::runtime_error("Mesh vertex count does not match buffer size!");  
   }  

   // 頂点バッファをマップして更新  
   D3D11_MAPPED_SUBRESOURCE mappedResource;  
   HR(context->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));  

   auto* vertices = reinterpret_cast<VertexPosColorTex*>(mappedResource.pData);  

   for (UINT i = 0; i < vertexCount; ++i)  
   {  
       vertices[i].Position = mesh.vertices[i];  
       vertices[i].Color = mesh.colors;  // 色データを更新  
       vertices[i].TexCoord = mesh.uv[i];  // テクスチャ座標を更新  
   }  

   context->Unmap(vertexBuffer.Get(), 0);  
}