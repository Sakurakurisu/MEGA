#pragma once

#include <vector>
#include <memory>
#include <cmath>
#include <algorithm>
#include <set>
#include <unordered_map>
#include <DirectXMath.h>
#include "BaseManager.h"

using namespace DirectX;

class GameObject;
class Collider2D;
class BoxCollider2D;
class CircleCollider2D;

class CollisionMgr : public BaseManager<CollisionMgr>
{
friend class BaseManager<CollisionMgr>;

public:
bool isDebug = false;

// 2Dコライダーを登録する
void RegisterCollider2D(Collider2D* coll);

// 2Dコライダーを登録解除する
void UnregisterCollider2D(Collider2D* coll);

// すべての2Dコライダーをクリアする
void ClearAllColliders2D();

// 衝突をチェックする
void CheckCollisions();

// デバッグ用のコライダーを表示する
void ShowDebugColliders();

private:
CollisionMgr() = default;
CollisionMgr(const CollisionMgr&) = delete;
CollisionMgr& operator=(const CollisionMgr&) = delete;

// 登録された2Dコライダーのリスト
std::vector<Collider2D*> colliders2D;

// 現在のフレームでの衝突情報
std::unordered_map<Collider2D*, std::set<Collider2D*>> currentCollisions2D;

// 前のフレームでの衝突情報
std::unordered_map<Collider2D*, std::set<Collider2D*>> previousCollisions2D;

// 2つのコライダー間の衝突をチェックする
bool CheckCollision(Collider2D& coll_a, Collider2D& coll_b);

// 2つのボックスコライダー間の衝突をチェックする
bool CheckBoxAndBoxCollision(const BoxCollider2D& box_a, const BoxCollider2D& box_b);

// 2つの円コライダー間の衝突をチェックする
bool CheckCircleAndCircleCollision(const CircleCollider2D& circle_a, const CircleCollider2D& circle_b);

// ボックスコライダーと円コライダー間の衝突をチェックする
bool CheckBoxAndCircleCollision(const BoxCollider2D& box, const CircleCollider2D& circle);

// 衝突後の応答を処理する
void HandleCollisionResponse(Collider2D* coll_a, Collider2D* coll_b);

// 衝突の詳細情報（法線と貫通深度）を計算する
bool ComputeCollisionManifold(Collider2D* coll_a, Collider2D* coll_b, XMFLOAT2& normal, float& penetration);
};