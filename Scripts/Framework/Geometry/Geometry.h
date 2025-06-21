#pragma once

#include <vector>
#include <string>
#include <map>
#include <functional>
#include "Vertex.h"
#include "UVRect.h"
#include "Mesh.h"

using namespace DirectX;

class Geometry
{
public:
// 正方形メッシュを作成します
static Mesh CreateSquareMesh(
	const XMFLOAT2& size = { 1.0f, 1.0f },
	const UVRect& uv = { 0.0f, 0.0f, 1.0f, 1.0f },
	const XMFLOAT2& center = { 0.0f, 0.0f },
	const XMFLOAT4& colors = { 1.0f, 1.0f, 1.0f, 1.0f }
);

// 円形メッシュを作成します
static Mesh CreateCircleMesh(
	float radius = 1.0f,
	uint32_t segments = 32,
	const XMFLOAT2& center = { 0.0f, 0.0f },
	const XMFLOAT4& colors = { 1.0f, 1.0f, 1.0f, 1.0f }
);

// 円形アウトラインメッシュを作成します
static Mesh CreateCircleOutlineMesh(
	float radius = 1.0f,
	uint32_t segments = 32,
	const XMFLOAT2& center = { 0.0f, 0.0f },
	const XMFLOAT4& colors = { 1.0f, 1.0f, 1.0f, 1.0f }
);

// 扇形メッシュを作成します
static Mesh CreateFanMesh(
	float radius = 1.0f,
	float range = 360.0f,
	uint32_t segments = 32,
	const XMFLOAT2& center = { 0.0f, 0.0f },
	const XMFLOAT4& colors = { 1.0f, 1.0f, 1.0f, 1.0f }
);

private:
struct VertexData
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT4 tangent;
	XMFLOAT4 color;
	XMFLOAT2 tex;
};

static void InsertVertexElement(VertexPosColorTex& vertexDst, const VertexData& vertexSrc);
};
