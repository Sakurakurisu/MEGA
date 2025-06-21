#pragma once

#include <d3d11.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

struct RenderStateConfig
{
	D3D11_PRIMITIVE_TOPOLOGY		topology;
	ComPtr<ID3D11InputLayout>       inputLayout = nullptr;
	ComPtr<ID3D11VertexShader>      vertexShader = nullptr;
	ComPtr<ID3D11PixelShader>       pixelShader = nullptr;
	ComPtr<ID3D11RasterizerState>	rasterizerState = nullptr;
	ComPtr<ID3D11BlendState>        blendState = nullptr;
	ComPtr<ID3D11DepthStencilState> depthStencilState = nullptr;
	UINT							stencilRef = 0;
	ComPtr<ID3D11SamplerState>      samplerState = nullptr;
};
