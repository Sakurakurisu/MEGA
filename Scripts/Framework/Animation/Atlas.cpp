#include "Atlas.h"
#include "Texture2D.h"
#include <stdexcept>

Atlas::Atlas(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& res, unsigned columns, unsigned rows)
{
	texture2D = std::make_shared<Texture2D>(res);
	if (rows == 0 || columns == 0)
	{
		throw std::invalid_argument("Rows and columns must be positive.");
	}
	UINT textureWidth = texture2D->GetWidth();
	UINT textureHeight = texture2D->GetHeight();
	spritesPerWidth = static_cast<float>(textureWidth) / static_cast<float>(columns);
	spritesPerHeight = static_cast<float>(textureHeight) / static_cast<float>(rows);
	// UV座標の生成（テクスチャを分割して各スプライトのUVを計算する）
	GenerateFrameUVs(textureWidth, textureHeight, spritesPerWidth, spritesPerHeight, rows, columns);

	sprites.reserve(uvs.size());
	for (const auto& uv : uvs)
	{
		auto sprite = std::make_shared<Sprite>(texture2D->GetShaderResourceView(), uv, XMFLOAT2(spritesPerWidth, spritesPerHeight));
		sprites.push_back(sprite);
	}
}

int Atlas::GetSpriteCount() const
{
	return static_cast<int>(uvs.size());
}

std::shared_ptr<Sprite> Atlas::GetSprite(unsigned int index) const
{
	if (index >= sprites.size())
		throw std::out_of_range("Sprite index out of range.");

	return sprites[index];
}

std::shared_ptr<Texture2D> Atlas::GetTexture2D() const
{
	return texture2D;
}

void Atlas::GenerateFrameUVs(const UINT textureWidth, const UINT textureHeight, const float spritesPerWidth, const float spritesPerHeight, const unsigned rows, const unsigned columns)
{
	// 2重ループで行・列ごとにUV座標を計算し、リストに追加する
	for (unsigned row = 0; row < rows; ++row)
	{
		for (unsigned col = 0; col < columns; ++col)
		{
			UVRect uv;
			uv.u0 = static_cast<float>(col * spritesPerWidth) / static_cast<float>(textureWidth);
			uv.v0 = static_cast<float>(row * spritesPerHeight) / static_cast<float>(textureHeight);
			uv.u1 = static_cast<float>((col + 1) * spritesPerWidth) / static_cast<float>(textureWidth);
			uv.v1 = static_cast<float>((row + 1) * spritesPerHeight) / static_cast<float>(textureHeight);

			uvs.push_back(uv);
		}
	}
}