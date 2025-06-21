#include "Collider2D.h"
#include "GameObject.h"
#include "CollisionMgr.h"
#include "SpriteRenderer.h"
#include "RenderSystem.h"
#include "D3DMgr.h"
#include "SceneMgr.h"
#include "Vector.h"
#include "Geometry.h"
#include <algorithm>
#include "Debug.h"
#include "RenderStateMgr.h"

#pragma region Collider2D

void Collider2D::DrawDebug(XMFLOAT4 color)
{
	// コライダーのワールド行列とオフセットを計算
	XMMATRIX worldMatrix = gameObject->transform.GetWorldMatrix();
	XMMATRIX offsetMatrix = XMMatrixTranslation(offset.x, offset.y, 0.0f);

	// デバッグ描画用のコマンド作成
	RenderCommand command;
	command.gameObject = gameObject;
	command.worldMatrix = worldMatrix * offsetMatrix;
	command.color = color;
	command.vertexBuffer = &debugVertexBuffer;
	command.mesh = debugMesh;
	command.renderStateConfig = RenderStateMgr::instance.GetRenderState("2DRenderWireframe").get();

	RenderSystem::instance.AddRenderCommand(command); // 描画キューに追加
}

void Collider2D::OnEnable()
{
	CollisionMgr::instance.RegisterCollider2D(this); // 有効時、コライダー登録
}

void Collider2D::OnDisable()
{
	CollisionMgr::instance.UnregisterCollider2D(this); // 無効時、登録解除
}

void Collider2D::OnDestroy()
{
	CollisionMgr::instance.UnregisterCollider2D(this); // 破棄時、登録解除
}

Collider2D::Collider2D() : debugVertexBuffer(D3DMgr::instance.GetDevice(), D3DMgr::instance.GetDeviceContext()) {}

XMFLOAT2 Collider2D::GetPosition() const
{
	// オフセット付きのワールド座標を返す
	XMFLOAT3 pos = gameObject->transform.position;
	return { pos.x + offset.x, pos.y + offset.y };
}

bool Collider2D::IsTrigger() const
{
	return isTrigger;
}

#pragma endregion

#pragma region BoxCollider2D

void BoxCollider2D::Init()
{
	// スプライトサイズに基づいてコライダーサイズを初期化
	SpriteRenderer* spriteRenderer = gameObject->GetComponent<SpriteRenderer>();

	if (spriteRenderer)
		SetSize(spriteRenderer->GetSprite()->GetWidth(), spriteRenderer->GetSprite()->GetHeight());
}

void BoxCollider2D::Start()
{
	// デバッグ描画用のメッシュを初期化（デバッグON時のみ）
	if (CollisionMgr::instance.isDebug)
	{
		debugMesh = Geometry::CreateSquareMesh(size);
		debugVertexBuffer.Init(debugMesh);
	}
}

XMFLOAT2 BoxCollider2D::GetSize() const
{
	// ゲームオブジェクトのスケールを反映したサイズを返す
	return { size.x * std::abs(gameObject->transform.scale.x),size.y * std::abs(gameObject->transform.scale.y) };
}

BoxCollider2D& BoxCollider2D::SetSize(float width, float height)
{
	size.x = width;
	size.y = height;

	// 手動設定後にメッシュ更新
	if (isSetSize)
	{
		debugMesh = Geometry::CreateSquareMesh(size);
		debugVertexBuffer.Init(debugMesh);
	}

	isSetSize = true;

	return *this;
}

BoxCollider2D& BoxCollider2D::SetOffset(float x, float y)
{
	offset.x = x;
	offset.y = y;

	return *this;
}

BoxCollider2D& BoxCollider2D::SetTrigger(bool enabled)
{
	isTrigger = enabled;

	return *this;
}

E_ColliderType BoxCollider2D::GetColliderType() const
{
	return E_ColliderType::Box2D;
}

#pragma endregion

#pragma region CircleCollider2D

void CircleCollider2D::Init()
{
	SpriteRenderer* spriteRenderer = gameObject->GetComponent<SpriteRenderer>();
	if (spriteRenderer)
	{
		// 最大幅か高さを半径として使用（円をスプライトに合わせる）
		float w = spriteRenderer->GetSprite()->GetWidth();
		float h = spriteRenderer->GetSprite()->GetHeight();
		SetRadius(std::max<float>(w, h) / 2.0f);
	}
}

void CircleCollider2D::Start()
{
	// デバッグ用の円メッシュ生成（点線）
	if (CollisionMgr::instance.isDebug)
	{
		debugMesh = Geometry::CreateCircleOutlineMesh(radius);
		debugVertexBuffer.Init(debugMesh);
	}
}

float CircleCollider2D::GetRadius() const
{
	// スケールを加味した実際の半径を返す
	float scaleAvg = (std::abs(gameObject->transform.scale.x) +
					  std::abs(gameObject->transform.scale.y)) / 2.0f;
	return radius * scaleAvg;
}

CircleCollider2D& CircleCollider2D::SetRadius(float r)
{
	radius = r;

	// メッシュの再初期化
	if (isSetRadius)
	{
		debugMesh = Geometry::CreateCircleOutlineMesh(radius);
		debugVertexBuffer.Init(debugMesh);
	}

	isSetRadius = true;

	return *this;
}

CircleCollider2D& CircleCollider2D::SetOffset(float x, float y)
{
	offset.x = x;
	offset.y = y;

	return *this;
}

CircleCollider2D& CircleCollider2D::SetTrigger(bool enabled)
{
	isTrigger = enabled;

	return *this;
}

E_ColliderType CircleCollider2D::GetColliderType() const
{
	return E_ColliderType::Circle2D;
}

#pragma endregion