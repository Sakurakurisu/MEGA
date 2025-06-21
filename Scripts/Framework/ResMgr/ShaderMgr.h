#pragma once  
#include <d3d11.h>  
#include <wrl/client.h>  
#include <string>  
#include <unordered_map>  
#include <memory>  
#include "BaseManager.h"  
#include <vector>  
#include "Vertex.h"  

using Microsoft::WRL::ComPtr;  

struct VertexShaderInfo  
{  
std::string id;  
std::wstring shaderFile;  
std::wstring shaderSource;  
std::string entryPoint;  
std::string shaderModel;  

const D3D11_INPUT_ELEMENT_DESC* layoutDesc;  
UINT numElements;  
};  

struct PixelShaderInfo  
{  
std::string id;  
std::wstring shaderFile;  
std::wstring shaderSource;  
std::string entryPoint;  
std::string shaderModel;  
};  

class ShaderMgr : public BaseManager<ShaderMgr>  
{  
friend class BaseManager<ShaderMgr>;  
public:  
// すべてのシェーダーを読み込む  
void LoadAllShaders(ID3D11Device* device);  

// 指定された頂点シェーダーを読み込む  
ComPtr<ID3D11VertexShader> LoadVertexShader(  
	ID3D11Device* device, const VertexShaderInfo& desc,  
	ComPtr<ID3D11InputLayout>& inputLayout);  

// 指定されたピクセルシェーダーを読み込む  
ComPtr<ID3D11PixelShader> LoadPixelShader(  
	ID3D11Device* device, const PixelShaderInfo& desc);  

// 頂点シェーダーを取得する  
ComPtr<ID3D11VertexShader> GetVertexShader(const std::string& id);  

// ピクセルシェーダーを取得する  
ComPtr<ID3D11PixelShader> GetPixelShader(const std::string& id);  

// 入力レイアウトを取得する  
ComPtr<ID3D11InputLayout> GetInputLayout(const std::string& id);  

// すべてのデバッグ名を設定する  
void SetAllDebugNames();  

private:  
ShaderMgr() = default;  
~ShaderMgr() = default;  
ShaderMgr(const ShaderMgr&) = delete;  
ShaderMgr& operator=(const ShaderMgr&) = delete;  

std::unordered_map<std::string, ComPtr<ID3D11VertexShader>> vertexShaderPool;  
std::unordered_map<std::string, ComPtr<ID3D11PixelShader>> pixelShaderPool;  
std::unordered_map<std::string, ComPtr<ID3D11InputLayout>> inputLayoutPool;  
};  

static const std::vector<VertexShaderInfo> vertexShaderInfos =  
{  
{ "Basic_2D_VS", L"HLSL\\Basic_2D_VS.cso", L"HLSL\\Basic_2D_VS.hlsl", "VS", "vs_5_0",VertexPosColorTex::inputLayout, ARRAYSIZE(VertexPosColorTex::inputLayout) }  
};  

static const std::vector<PixelShaderInfo> pixelShaderInfos =  
{  
{ "Basic_2D_PS", L"HLSL\\Basic_2D_PS.cso", L"HLSL\\Basic_2D_PS.hlsl", "PS", "ps_5_0" },  
{ "Wireframe_2D_PS", L"HLSL\\Wireframe_2D_PS.cso", L"HLSL\\Wireframe_2D_PS.hlsl", "PS", "ps_5_0" },  
{ "Grayscale_2D_PS", L"HLSL\\Grayscale_2D_PS.cso", L"HLSL\\Grayscale_2D_PS.hlsl", "PS", "ps_5_0" },  
{ "Flashable_2D_PS", L"HLSL\\Flashable_2D_PS.cso", L"HLSL\\Flashable_2D_PS.hlsl", "PS", "ps_5_0" },  
};