#include "Geometry.h"

// 頂点の各要素（位置・色・UV）を入力レイアウトに従って設定する
void Geometry::InsertVertexElement(VertexPosColorTex& vertexDst, const VertexData& vertexSrc)
{
	for (const auto& i : VertexPosColorTex::inputLayout)
	{
		const std::string semanticName = i.SemanticName;

		if (semanticName == "POSITION") // 位置データ
		{
			vertexDst.Position = XMFLOAT3(vertexSrc.pos.x, vertexSrc.pos.y, vertexSrc.pos.z);
		}
		else if (semanticName == "COLOR") // 色データ
		{
			vertexDst.Color = vertexSrc.color;
		}
		else if (semanticName == "TEXCOORD") // UV座標
		{
			vertexDst.TexCoord = vertexSrc.tex;
		}
	}
}

// 矩形メッシュの作成
Mesh Geometry::CreateSquareMesh(const XMFLOAT2& size, const UVRect& uv, const XMFLOAT2& center, const XMFLOAT4& colors)
{
	Mesh mesh;

	mesh.vertices.resize(4);
	mesh.uv.resize(4);
	mesh.colors = colors;

	// 矩形の4頂点の位置を計算
	std::vector<XMFLOAT3> positions = {
		{center.x - size.x * 0.5f, center.y - size.y * 0.5f, 0.0f},
		{center.x - size.x * 0.5f, center.y + size.y * 0.5f, 0.0f},
		{center.x + size.x * 0.5f, center.y + size.y * 0.5f, 0.0f},
		{center.x + size.x * 0.5f, center.y - size.y * 0.5f, 0.0f}
	};

	// UV座標を設定
	std::vector<XMFLOAT2> texCoords = {
		{uv.u0, uv.v0},
		{uv.u0, uv.v1},
		{uv.u1, uv.v1},
		{uv.u1, uv.v0}
	};

	for (size_t i = 0; i < 4; ++i)
	{
		VertexPosColorTex vertex{};
		VertexData vd{
			positions[i],
			{},
			{},
			colors,
			texCoords[i]
		};

		InsertVertexElement(vertex, vd);

		mesh.vertices[i] = vertex.Position;
		mesh.uv[i] = vertex.TexCoord;
	}

	// インデックスで2つの三角形を構成（矩形）
	mesh.triangles = { 0, 1, 2, 2, 3, 0 };

	return mesh;
}

// 円形メッシュの作成（中央から放射状の三角形で構成）
Mesh Geometry::CreateCircleMesh(float radius, uint32_t segments, const XMFLOAT2& center, const XMFLOAT4& colors)
{
	Mesh mesh;

	const uint32_t vertexCount = segments + 1;
	mesh.vertices.resize(vertexCount);
	mesh.uv.resize(vertexCount);
	mesh.colors = colors;

	// 中心点を最初の頂点として追加
	mesh.vertices[0] = XMFLOAT3(center.x, center.y, 0.0f);
	mesh.uv[0] = XMFLOAT2(0.5f, 0.5f);

	const float angleStep = XM_2PI / segments;
	for (uint32_t i = 0; i < segments; ++i)
	{
		// 各角度に応じて円周上の頂点を配置
		float angle = i * angleStep;
		float x = center.x + radius * cosf(angle);
		float y = center.y + radius * sinf(angle);

		mesh.vertices[i + 1] = XMFLOAT3(x, y, 0.0f);

		// UV座標を0.5中心で正規化
		float u = cosf(angle) * 0.5f + 0.5f;
		float v = sinf(angle) * 0.5f + 0.5f;
		mesh.uv[i + 1] = XMFLOAT2(u, v);
	}

	// 扇形状に三角形を構成
	mesh.triangles.reserve(segments * 3);
	for (uint32_t i = 0; i < segments; ++i)
	{
		mesh.triangles.push_back(0);
		mesh.triangles.push_back(i + 1);
		mesh.triangles.push_back(((i + 1) % segments) + 1);
	}

	return mesh;
}


// 円の輪郭のみを描画するメッシュ
Mesh Geometry::CreateCircleOutlineMesh(float radius, uint32_t segments, const XMFLOAT2& center, const XMFLOAT4& colors)
{
	Mesh mesh;

	mesh.vertices.resize(segments);
	mesh.uv.resize(segments);
	mesh.colors = colors;

	const float angleStep = XM_2PI / segments;
	for (uint32_t i = 0; i < segments; ++i)
	{
		// 円周上の各点を計算
		float angle = i * angleStep;
		float x = center.x + radius * cosf(angle);
		float y = center.y + radius * sinf(angle);

		mesh.vertices[i] = XMFLOAT3(x, y, 0.0f);

		float u = cosf(angle) * 0.5f + 0.5f;
		float v = sinf(angle) * 0.5f + 0.5f;
		mesh.uv[i] = XMFLOAT2(u, v);
	}

	// 線を構成するインデックス（隣接頂点を接続）
	mesh.triangles.reserve(segments * 2);
	for (uint32_t i = 0; i < segments; ++i)
	{
		mesh.triangles.push_back(i);
		mesh.triangles.push_back((i + 1) % segments);
	}

	return mesh;
}

// 扇形のメッシュ作成（中心点から角度範囲内に展開）
Mesh Geometry::CreateFanMesh(float radius, float range, uint32_t segments, const XMFLOAT2& center, const XMFLOAT4& colors)
{
	Mesh mesh;

	const uint32_t vertexCount = segments + 1;
	mesh.vertices.resize(vertexCount);
	mesh.uv.resize(vertexCount);
	mesh.colors = colors;

	// 中心点を追加
	mesh.vertices[0] = XMFLOAT3(center.x, center.y, 0.0f);
	mesh.uv[0] = XMFLOAT2(0.5f, 0.5f);

	const float angleStep = XMConvertToRadians(range) / segments;
	const float startAngle = XMConvertToRadians(-range / 2);

	for (uint32_t i = 0; i < segments; ++i)
	{
		// 扇形の端から端まで分割して配置
		float angle = i * angleStep + startAngle;
		float x = center.x + radius * cosf(angle);
		float y = center.y + radius * sinf(angle);

		mesh.vertices[i + 1] = XMFLOAT3(x, y, 0.0f);

		float u = cosf(angle) * 0.5f + 0.5f;
		float v = sinf(angle) * 0.5f + 0.5f;
		mesh.uv[i + 1] = XMFLOAT2(u, v);
	}

	// 扇形の三角形構成（最終セグメントは含まない）
	mesh.triangles.reserve(segments * 3);
	for (uint32_t i = 0; i < segments - 1; ++i)
	{
		mesh.triangles.push_back(0);
		mesh.triangles.push_back(i + 1);
		mesh.triangles.push_back(((i + 1) % segments) + 1);
	}
	return mesh;
}