#pragma once

#include <DirectXMath.h>
#include <d3d11.h>

using namespace DirectX;

// 頂点構造体
struct VertexPosColorTex
{
	XMFLOAT3 Position;
	XMFLOAT4 Color;
	XMFLOAT2 TexCoord;

	static const D3D11_INPUT_ELEMENT_DESC inputLayout[3];

	VertexPosColorTex() = default;

	constexpr VertexPosColorTex(const XMFLOAT3& pos, const XMFLOAT4 color, const XMFLOAT2 tex) : Position(pos), Color(color), TexCoord(tex) {}
};