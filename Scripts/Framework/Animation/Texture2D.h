#pragma once

#include <d3d11.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class Texture2D
{
public:
	Texture2D(const ComPtr<ID3D11ShaderResourceView>& srv);

	Texture2D(const Texture2D&) = delete;
	Texture2D& operator=(const Texture2D&) = delete;

	const ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() const;

	UINT GetWidth() const;

	UINT GetHeight() const;

private:
	ComPtr<ID3D11ShaderResourceView> textureSRV;
	UINT width = 0;
	UINT height = 0;
};
