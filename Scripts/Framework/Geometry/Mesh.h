#pragma once

#include <vector>
#include <DirectXMath.h>
#include <cstdint>

using namespace DirectX;

struct Mesh
{
public:
	std::vector<XMFLOAT3> vertices;
	std::vector<XMFLOAT2> uv;
	std::vector<uint32_t> triangles;
	XMFLOAT4 colors{};
};
