#include "CollisionMgr.h"
#include "Collision2D.h"
#include "Vector.h"
#include "LayerCollisionMatrix.h"

void CollisionMgr::CheckCollisions()
{
	// 現在のフレームの衝突記録をクリア
	currentCollisions2D.clear();

	for (auto it_a = colliders2D.begin(); it_a != colliders2D.end(); ++it_a)
	{
		Collider2D* coll_a = *it_a;
		Rigidbody2D* rb_a = coll_a->gameObject->GetComponent<Rigidbody2D>();
		bool hasRigidbodyA = rb_a != nullptr;
		bool isTriggerA = coll_a->IsTrigger();

		for (auto it_b = std::next(it_a); it_b != colliders2D.end(); ++it_b)
		{
			Collider2D* coll_b = *it_b;
			Rigidbody2D* rb_b = coll_b->gameObject->GetComponent<Rigidbody2D>();
			bool hasRigidbodyB = rb_b != nullptr;
			bool isTriggerB = coll_b->IsTrigger();

			// 両方とも Rigidbody2D を持っていない場合はスキップ（必要に応じて）
			if (!hasRigidbodyA && !hasRigidbodyB)
				continue;

			// 衝突をチェック
			if (CheckCollision(*coll_a, *coll_b))
			{
				currentCollisions2D[coll_a].insert(coll_b);
				currentCollisions2D[coll_b].insert(coll_a);

				bool isNewCollision = previousCollisions2D[coll_a].find(coll_b) == previousCollisions2D[coll_a].end();

				// 衝突情報を計算
				XMFLOAT2 normal;
				float penetration;
				ComputeCollisionManifold(coll_a, coll_b, normal, penetration);

				// 衝突点を計算
				XMFLOAT2 contactPoint;
				contactPoint.x = coll_a->GetPosition().x + normal.x * penetration * 0.5f;
				contactPoint.y = coll_a->GetPosition().y + normal.y * penetration * 0.5f;

				// 衝突情報オブジェクトを作成
				Collision2D collisionInfoA(*coll_b, normal, penetration, contactPoint);
				XMFLOAT2 reversedNormal = XMFLOAT2(-normal.x, -normal.y);
				Collision2D collisionInfoB(*coll_a, reversedNormal, penetration, contactPoint);

				if (isTriggerA || isTriggerB)
				{
					// 両方に Trigger イベントを発火
					if (isNewCollision)
					{
						coll_a->gameObject->OnTriggerEnter2D(collisionInfoA);
						coll_b->gameObject->OnTriggerEnter2D(collisionInfoB);
					}
					else
					{
						coll_a->gameObject->OnTriggerStay2D(collisionInfoA);
						coll_b->gameObject->OnTriggerStay2D(collisionInfoB);
					}
				}
				else
				{
					// 両方に Collision イベントを発火
					if (isNewCollision)
					{
						coll_a->gameObject->OnCollisionEnter2D(collisionInfoA);
						coll_b->gameObject->OnCollisionEnter2D(collisionInfoB);
					}
					else
					{
						coll_a->gameObject->OnCollisionStay2D(collisionInfoA);
						coll_b->gameObject->OnCollisionStay2D(collisionInfoB);
					}

					// 物理的な衝突応答を処理
					HandleCollisionResponse(coll_a, coll_b);
				}
			}
		}
	}

	// 衝突終了イベントを処理
	auto tempCollisions = previousCollisions2D; // 一時的にコピー
	for (const auto& [collider, collidingSet] : tempCollisions)
	{
		Collider2D* coll = collider;
		Rigidbody2D* rb_coll = coll->gameObject->GetComponent<Rigidbody2D>();
		bool hasRigidbodyColl = rb_coll != nullptr;
		bool isTriggerColl = coll->IsTrigger();

		auto tempSet = collidingSet; // 一時的にコピー
		for (Collider2D* other : tempSet)
		{
			if (currentCollisions2D[coll].find(other) == currentCollisions2D[coll].end())
			{
				Rigidbody2D* rb_other = other->gameObject->GetComponent<Rigidbody2D>();
				bool hasRigidbodyOther = rb_other != nullptr;
				bool isTriggerOther = other->IsTrigger();

				// 両方とも Rigidbody2D を持っていない場合はスキップ（必要に応じて）
				if (!hasRigidbodyColl && !hasRigidbodyOther)
					continue;

				// 終了イベントのため、法線はゼロベクトルの衝突情報を使用
				Collision2D collisionInfoColl(*other);
				Collision2D collisionInfoOther(*coll);

				if (isTriggerColl || isTriggerOther)
				{
					// 両方に OnTriggerExit イベントを発火
					coll->gameObject->OnTriggerExit2D(collisionInfoColl);
					other->gameObject->OnTriggerExit2D(collisionInfoOther);
				}
				else
				{
					// 両方に OnCollisionExit イベントを発火
					coll->gameObject->OnCollisionExit2D(collisionInfoColl);
					other->gameObject->OnCollisionExit2D(collisionInfoOther);
				}
			}
		}
	}

	// 前フレームの衝突情報を更新
	previousCollisions2D = currentCollisions2D;
}


void CollisionMgr::RegisterCollider2D(Collider2D* coll)
{
	colliders2D.push_back(coll);
}

void CollisionMgr::UnregisterCollider2D(Collider2D* coll)
{
	auto it = std::find(colliders2D.begin(), colliders2D.end(), coll);
	if (it != colliders2D.end())

		colliders2D.erase(it);

	currentCollisions2D.erase(coll);

	previousCollisions2D.erase(coll);

	for (auto& [key, collidingSet] : currentCollisions2D)
	{
		if (collidingSet.find(coll) != collidingSet.end())
			collidingSet.erase(coll);
	}

	for (auto& [key, collidingSet] : previousCollisions2D)
	{
		if (collidingSet.find(coll) != collidingSet.end())

			collidingSet.erase(coll);
	}
}

void CollisionMgr::ClearAllColliders2D()
{
	colliders2D.clear();
	//currentCollisions2D.clear();
	//previousCollisions2D.clear();
}

bool CollisionMgr::CheckCollision(Collider2D& coll_a, Collider2D& coll_b)
{
	if (!LayerCollisionMatrix::ShouldCollide(coll_a.gameObject->layer, coll_b.gameObject->layer))
		return false;

	// コライダーのタイプを取得
	E_ColliderType type_a = coll_a.GetColliderType();
	E_ColliderType type_b = coll_b.GetColliderType();

	// タイプの組み合わせに応じて適切な衝突検出アルゴリズムを選択
	if (type_a == E_ColliderType::Box2D && type_b == E_ColliderType::Box2D)
	{
		return CheckBoxAndBoxCollision(static_cast<BoxCollider2D&>(coll_a), static_cast<BoxCollider2D&>(coll_b));
	}
	else if (type_a == E_ColliderType::Circle2D && type_b == E_ColliderType::Circle2D)
	{
		return CheckCircleAndCircleCollision(static_cast<CircleCollider2D&>(coll_a), static_cast<CircleCollider2D&>(coll_b));
	}
	else if ((type_a == E_ColliderType::Box2D && type_b == E_ColliderType::Circle2D))
	{
		return CheckBoxAndCircleCollision(static_cast<BoxCollider2D&>(coll_a), static_cast<CircleCollider2D&>(coll_b));
	}
	else if ((type_a == E_ColliderType::Circle2D && type_b == E_ColliderType::Box2D))
	{
		return CheckBoxAndCircleCollision(static_cast<BoxCollider2D&>(coll_b), static_cast<CircleCollider2D&>(coll_a));
	}

	return false;
}

bool CollisionMgr::CheckBoxAndBoxCollision(const BoxCollider2D& box_a, const BoxCollider2D& box_b)
{
	// AABBアルゴリズムを使ってボックスとボックスの衝突を検出する
	return (std::abs(box_a.GetPosition().x - box_b.GetPosition().x) <= (box_a.GetSize().x / 2 + box_b.GetSize().x / 2)) &&
		(std::abs(box_a.GetPosition().y - box_b.GetPosition().y) <= (box_a.GetSize().y / 2 + box_b.GetSize().y / 2));
}

bool CollisionMgr::CheckCircleAndCircleCollision(const CircleCollider2D& circle_a, const CircleCollider2D& circle_b)
{
	float distanceSquared = (circle_a.GetPosition().x - circle_b.GetPosition().x) * (circle_a.GetPosition().x - circle_b.GetPosition().x) +
		(circle_a.GetPosition().y - circle_b.GetPosition().y) * (circle_a.GetPosition().y - circle_b.GetPosition().y);
	float radiusSum = circle_a.GetRadius() + circle_b.GetRadius();
	return distanceSquared <= (radiusSum * radiusSum);
}

bool CollisionMgr::CheckBoxAndCircleCollision(const BoxCollider2D& box, const CircleCollider2D& circle)
{
	float closestX = std::fmax(box.GetPosition().x - box.GetSize().x / 2, std::fmin(circle.GetPosition().x, box.GetPosition().x + box.GetSize().x / 2));
	float closestY = std::fmax(box.GetPosition().y - box.GetSize().y / 2, std::fmin(circle.GetPosition().y, box.GetPosition().y + box.GetSize().y / 2));

	float distanceX = circle.GetPosition().x - closestX;
	float distanceY = circle.GetPosition().y - closestY;

	return (distanceX * distanceX + distanceY * distanceY) <= (circle.GetRadius() * circle.GetRadius());
}

void CollisionMgr::HandleCollisionResponse(Collider2D* coll_a, Collider2D* coll_b)
{
	if (coll_a->IsTrigger() || coll_b->IsTrigger())
	{
		return;
	}

	Rigidbody2D* rb_a = coll_a->gameObject->GetComponent<Rigidbody2D>();
	Rigidbody2D* rb_b = coll_b->gameObject->GetComponent<Rigidbody2D>();

	// 少なくとも1つのオブジェクトが動的であることを確認（リジッドボディを持ち、キネマティックでない）
	if ((rb_a == nullptr || rb_a->IsKinematic()) && (rb_b == nullptr || rb_b->IsKinematic()))
		return;

	// 衝突法線と貫通深度を計算
	XMFLOAT2 normal{};
	float penetration;
	if (!ComputeCollisionManifold(coll_a, coll_b, normal, penetration))
		return; // 計算できない場合は処理を中止

	// 位置補正
	float invMassA = (rb_a && !rb_a->IsKinematic()) ? 1.0f / rb_a->GetMass() : 0.0f;
	float invMassB = (rb_b && !rb_b->IsKinematic()) ? 1.0f / rb_b->GetMass() : 0.0f;
	float totalInverseMass = invMassA + invMassB;

	// ゼロ除算を防ぐ
	if (totalInverseMass <= 0.0f)
		return;

	// 補正量を計算
	XMFLOAT2 correction = normal * (penetration / totalInverseMass);

	// 位置補正を適用
	if (invMassA > 0.0f)
		rb_a->gameObject->transform.position -= correction * invMassA;
	if (invMassB > 0.0f)
		rb_b->gameObject->transform.position += correction * invMassB;

	// 相対速度を計算
	XMFLOAT2 velocityA = (rb_a && !rb_a->IsKinematic()) ? rb_a->GetVelocity() : XMFLOAT2(0.0f, 0.0f);
	XMFLOAT2 velocityB = (rb_b && !rb_b->IsKinematic()) ? rb_b->GetVelocity() : XMFLOAT2(0.0f, 0.0f);
	XMFLOAT2 relativeVelocity = velocityA - velocityB;

	// 法線方向の速度成分を計算
	float velocityAlongNormal = Dot(relativeVelocity, normal);

	//if (velocityAlongNormal > 1e-4f)
	//	return; // オブジェクトが離れている場合は処理不要

	// 反発係数による衝突インパルス（法線方向のみ）を計算
	float restitution = 0.0f; // 反発係数（必要に応じて設定）
	float impulseScalar = -(1 + restitution) * velocityAlongNormal / totalInverseMass;
	XMFLOAT2 impulse = normal * impulseScalar;

	// インパルスを適用
	if (invMassA > 0.0f)
	{
		// 速度を分解
		XMFLOAT2 vA = velocityA;
		float vnA = Dot(vA, normal);
		XMFLOAT2 vnA_vec = normal * vnA;
		XMFLOAT2 vtA = vA - vnA_vec;

		// 法線速度を更新
		vnA_vec += impulse * invMassA;

		// 新しい速度を合成
		XMFLOAT2 newVelocityA = vtA + vnA_vec;
		rb_a->SetVelocity(newVelocityA);
	}
	if (invMassB > 0.0f)
	{
		// 速度を分解
		XMFLOAT2 vB = velocityB;
		float vnB = Dot(vB, normal);
		XMFLOAT2 vnB_vec = normal * vnB;
		XMFLOAT2 vtB = vB - vnB_vec;

		// 法線速度を更新
		vnB_vec -= impulse * invMassB;

		// 新しい速度を合成
		XMFLOAT2 newVelocityB = vtB + vnB_vec;
		rb_b->SetVelocity(newVelocityB);
	}
}


bool CollisionMgr::ComputeCollisionManifold(Collider2D* coll_a, Collider2D* coll_b, XMFLOAT2& normal, float& penetration)
{
	// コライダーのタイプを取得
	E_ColliderType type_a = coll_a->GetColliderType();
	E_ColliderType type_b = coll_b->GetColliderType();

	// 円形同士の衝突
	if (type_a == E_ColliderType::Circle2D && type_b == E_ColliderType::Circle2D)
	{
		CircleCollider2D* circle_a = static_cast<CircleCollider2D*>(coll_a);
		CircleCollider2D* circle_b = static_cast<CircleCollider2D*>(coll_b);

		XMFLOAT2 posA = circle_a->GetPosition();
		XMFLOAT2 posB = circle_b->GetPosition();

		// AからBへのベクトルを計算
		XMFLOAT2 delta = { posB.x - posA.x, posB.y - posA.y };
		float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);

		// 距離が0の場合、ゼロ除算を避ける
		if (distance == 0.0f)
		{
			normal = { 1.0f, 0.0f }; // デフォルト方向
			penetration = circle_a->GetRadius() + circle_b->GetRadius();
		}
		else
		{
			normal = { delta.x / distance, delta.y / distance };
			penetration = circle_a->GetRadius() + circle_b->GetRadius() - distance;
		}

		return penetration > 0;
	}

	// 矩形同士の衝突
	else if (type_a == E_ColliderType::Box2D && type_b == E_ColliderType::Box2D)
	{
		BoxCollider2D* box_a = static_cast<BoxCollider2D*>(coll_a);
		BoxCollider2D* box_b = static_cast<BoxCollider2D*>(coll_b);

		XMFLOAT2 posA = box_a->GetPosition();
		XMFLOAT2 posB = box_b->GetPosition();
		XMFLOAT2 halfSizeA = box_a->GetSize() * 0.5f;
		XMFLOAT2 halfSizeB = box_b->GetSize() * 0.5f;

		float dx = posB.x - posA.x;
		float px = (halfSizeA.x + halfSizeB.x) - std::abs(dx);
		if (px <= 0) return false;

		float dy = posB.y - posA.y;
		float py = (halfSizeA.y + halfSizeB.y) - std::abs(dy);
		if (py <= 0) return false;

		if (px < py)
		{
			normal = { dx < 0 ? -1.0f : 1.0f, 0.0f };
			penetration = px;
		}
		else
		{
			normal = { 0.0f, dy < 0 ? -1.0f : 1.0f };
			penetration = py;
		}

		return true;
	}

	// 矩形と円の衝突
	else if ((type_a == E_ColliderType::Box2D && type_b == E_ColliderType::Circle2D) ||
			 (type_a == E_ColliderType::Circle2D && type_b == E_ColliderType::Box2D))
	{
		// box_aが矩形で、circle_bが円形になるようにする
		BoxCollider2D* box;
		CircleCollider2D* circle;
		bool swapped = false;

		if (type_a == E_ColliderType::Box2D)
		{
			box = static_cast<BoxCollider2D*>(coll_a);
			circle = static_cast<CircleCollider2D*>(coll_b);
		}
		else
		{
			box = static_cast<BoxCollider2D*>(coll_b);
			circle = static_cast<CircleCollider2D*>(coll_a);
			swapped = true;
		}

		XMFLOAT2 boxPos = box->GetPosition();
		XMFLOAT2 boxHalfSize = box->GetSize() * 0.5f;
		XMFLOAT2 circlePos = circle->GetPosition();

		// 矩形上の最も近い点を見つける
		XMFLOAT2 closestPoint = {
			std::clamp(circlePos.x, boxPos.x - boxHalfSize.x, boxPos.x + boxHalfSize.x),
			std::clamp(circlePos.y, boxPos.y - boxHalfSize.y, boxPos.y + boxHalfSize.y)
		};

		// 円の中心から最近点へのベクトルを計算
		XMFLOAT2 delta = {
			circlePos.x - closestPoint.x,
			circlePos.y - closestPoint.y
		};

		float distanceSquared = delta.x * delta.x + delta.y * delta.y;

		// 円の中心が矩形の内部にある場合
		if (distanceSquared == 0.0f)
		{
			// 最小の貫通方向を見つける
			XMFLOAT2 depths = {
				boxHalfSize.x - std::abs(circlePos.x - boxPos.x),
				boxHalfSize.y - std::abs(circlePos.y - boxPos.y)
			};

			if (depths.x < depths.y)
			{
				normal = { (circlePos.x > boxPos.x) ? 1.0f : -1.0f, 0.0f };
				penetration = depths.x;
			}
			else
			{
				normal = { 0.0f, (circlePos.y > boxPos.y) ? 1.0f : -1.0f };
				penetration = depths.y;
			}
		}
		else
		{
			float distance = std::sqrt(distanceSquared);
			normal = { delta.x / distance, delta.y / distance };
			penetration = circle->GetRadius() - distance;
		}

		// 衝突体が入れ替わっていた場合、法線の向きを反転する
		if (swapped)
		{
			normal = { -normal.x, -normal.y };
		}

		return penetration > 0;
	}

	return false;
}

void CollisionMgr::ShowDebugColliders()
{
	if (isDebug)
	{
		for (Collider2D* collider : colliders2D)
		{
			bool isColliding = currentCollisions2D.find(collider) != currentCollisions2D.end();

			XMFLOAT4 color = isColliding ? XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)
				: XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

			collider->DrawDebug(color);
		}
	}
    #include "CollisionMgr.h"  
    #include "Collision2D.h"  
    #include "Vector.h"  
    #include "LayerCollisionMatrix.h"  

    void CollisionMgr::CheckCollisions()  
    {  
    // 現フレームの衝突記録をクリア  
    currentCollisions2D.clear();  

    for (auto it_a = colliders2D.begin(); it_a != colliders2D.end(); ++it_a)  
    {  
					Collider2D* coll_a = *it_a;  
					Rigidbody2D* rb_a = coll_a->gameObject->GetComponent<Rigidbody2D>();  
					bool hasRigidbodyA = rb_a != nullptr;  
					bool isTriggerA = coll_a->IsTrigger();  

					for (auto it_b = std::next(it_a); it_b != colliders2D.end(); ++it_b)  
					{  
						Collider2D* coll_b = *it_b;  
						Rigidbody2D* rb_b = coll_b->gameObject->GetComponent<Rigidbody2D>();  
						bool hasRigidbodyB = rb_b != nullptr;  
						bool isTriggerB = coll_b->IsTrigger();  

						// 両方のオブジェクトにRigidbody2Dがない場合、スキップ（必要に応じて）  
						if (!hasRigidbodyA && !hasRigidbodyB)  
							continue;  

						// 衝突をチェック  
						if (CheckCollision(*coll_a, *coll_b))  
						{  
							currentCollisions2D[coll_a].insert(coll_b);  
							currentCollisions2D[coll_b].insert(coll_a);  

							bool isNewCollision = previousCollisions2D[coll_a].find(coll_b) == previousCollisions2D[coll_a].end();  

							// 衝突情報を計算  
							XMFLOAT2 normal;  
							float penetration;  
							ComputeCollisionManifold(coll_a, coll_b, normal, penetration);  

							// 衝突点を計算  
							XMFLOAT2 contactPoint;  
							contactPoint.x = coll_a->GetPosition().x + normal.x * penetration * 0.5f;  
							contactPoint.y = coll_a->GetPosition().y + normal.y * penetration * 0.5f;  

							// 衝突情報オブジェクトを作成  
							Collision2D collisionInfoA(*coll_b, normal, penetration, contactPoint);  
							XMFLOAT2 reversedNormal = XMFLOAT2(-normal.x, -normal.y);  
							Collision2D collisionInfoB(*coll_a, reversedNormal, penetration, contactPoint);  

							if (isTriggerA || isTriggerB)  
							{  
								// 両方のTriggerイベントを発火  
								if (isNewCollision)  
								{  
									coll_a->gameObject->OnTriggerEnter2D(collisionInfoA);  
									coll_b->gameObject->OnTriggerEnter2D(collisionInfoB);  
								}  
								else  
								{  
									coll_a->gameObject->OnTriggerStay2D(collisionInfoA);  
									coll_b->gameObject->OnTriggerStay2D(collisionInfoB);  
								}  
							}  
							else  
							{  
								// 両方のCollisionイベントを発火  
								if (isNewCollision)  
								{  
									coll_a->gameObject->OnCollisionEnter2D(collisionInfoA);  
									coll_b->gameObject->OnCollisionEnter2D(collisionInfoB);  
								}  
								else  
								{  
									coll_a->gameObject->OnCollisionStay2D(collisionInfoA);  
									coll_b->gameObject->OnCollisionStay2D(collisionInfoB);  
								}  

								// 物理的な衝突応答を処理  
								HandleCollisionResponse(coll_a, coll_b);  
							}  
						}  
					}  
    }  

    // 衝突終了イベントを処理  
    auto tempCollisions = previousCollisions2D; // コピーを作成  
    for (const auto& [collider, collidingSet] : tempCollisions)  
    {  
					Collider2D* coll = collider;  
					Rigidbody2D* rb_coll = coll->gameObject->GetComponent<Rigidbody2D>();  
					bool hasRigidbodyColl = rb_coll != nullptr;  
					bool isTriggerColl = coll->IsTrigger();  

					auto tempSet = collidingSet; // コピーを作成  
					for (Collider2D* other : tempSet)  
					{  
						if (currentCollisions2D[coll].find(other) == currentCollisions2D[coll].end())  
						{  
							Rigidbody2D* rb_other = other->gameObject->GetComponent<Rigidbody2D>();  
							bool hasRigidbodyOther = rb_other != nullptr;  
							bool isTriggerOther = other->IsTrigger();  

							// 両方のオブジェクトにRigidbody2Dがない場合、スキップ（必要に応じて）  
							if (!hasRigidbodyColl && !hasRigidbodyOther)  
								continue;  

							// 終了イベントではデフォルトの衝突情報を使用（法線はゼロベクトル）  
							Collision2D collisionInfoColl(*other);  
							Collision2D collisionInfoOther(*coll);  

							if (isTriggerColl || isTriggerOther)  
							{  
								// 両方のOnTriggerExitイベントを発火  
								coll->gameObject->OnTriggerExit2D(collisionInfoColl);  
								other->gameObject->OnTriggerExit2D(collisionInfoOther);  
							}  
							else  
							{  
								// 両方のOnCollisionExitイベントを発火  
								coll->gameObject->OnCollisionExit2D(collisionInfoColl);  
								other->gameObject->OnCollisionExit2D(collisionInfoOther);  
							}  
						}  
					}  
    }  

    // 前フレームの衝突情報を更新  
    previousCollisions2D = currentCollisions2D;  
    }  

    void CollisionMgr::RegisterCollider2D(Collider2D* coll)  
    {  
    colliders2D.push_back(coll);  
    }  

    void CollisionMgr::UnregisterCollider2D(Collider2D* coll)  
    {  
    auto it = std::find(colliders2D.begin(), colliders2D.end(), coll);  
    if (it != colliders2D.end())  

					colliders2D.erase(it);  

    currentCollisions2D.erase(coll);  

    previousCollisions2D.erase(coll);  

    for (auto& [key, collidingSet] : currentCollisions2D)  
    {  
					if (collidingSet.find(coll) != collidingSet.end())  
						collidingSet.erase(coll);  
    }  

    for (auto& [key, collidingSet] : previousCollisions2D)  
    {  
					if (collidingSet.find(coll) != collidingSet.end())  

						collidingSet.erase(coll);  
    }  
    }  

    void CollisionMgr::ClearAllColliders2D()  
    {  
    colliders2D.clear();  
    //currentCollisions2D.clear();  
    //previousCollisions2D.clear();  
    }  

    bool CollisionMgr::CheckCollision(Collider2D& coll_a, Collider2D& coll_b)  
    {  
    if (!LayerCollisionMatrix::ShouldCollide(coll_a.gameObject->layer, coll_b.gameObject->layer))  
					return false;  

    // 衝突体のタイプを取得  
    E_ColliderType type_a = coll_a.GetColliderType();  
    E_ColliderType type_b = coll_b.GetColliderType();  

    // タイプの組み合わせに応じて適切な衝突検出アルゴリズムを選択  
    if (type_a == E_ColliderType::Box2D && type_b == E_ColliderType::Box2D)  
    {  
					return CheckBoxAndBoxCollision(static_cast<BoxCollider2D&>(coll_a), static_cast<BoxCollider2D&>(coll_b));  
    }  
    else if (type_a == E_ColliderType::Circle2D && type_b == E_ColliderType::Circle2D)  
    {  
					return CheckCircleAndCircleCollision(static_cast<CircleCollider2D&>(coll_a), static_cast<CircleCollider2D&>(coll_b));  
    }  
    else if ((type_a == E_ColliderType::Box2D && type_b == E_ColliderType::Circle2D))  
    {  
					return CheckBoxAndCircleCollision(static_cast<BoxCollider2D&>(coll_a), static_cast<CircleCollider2D&>(coll_b));  
    }  
    else if ((type_a == E_ColliderType::Circle2D && type_b == E_ColliderType::Box2D))  
    {  
					return CheckBoxAndCircleCollision(static_cast<BoxCollider2D&>(coll_b), static_cast<CircleCollider2D&>(coll_a));  
    }  

    return false;  
    }  

    bool CollisionMgr::CheckBoxAndBoxCollision(const BoxCollider2D& box_a, const BoxCollider2D& box_b)  
    {  
    // AABBアルゴリズムを使用してボックス同士の衝突を検出  
    return (std::abs(box_a.GetPosition().x - box_b.GetPosition().x) <= (box_a.GetSize().x / 2 + box_b.GetSize().x / 2)) &&  
					(std::abs(box_a.GetPosition().y - box_b.GetPosition().y) <= (box_a.GetSize().y / 2 + box_b.GetSize().y / 2));  
    }  

    bool CollisionMgr::CheckCircleAndCircleCollision(const CircleCollider2D& circle_a, const CircleCollider2D& circle_b)  
    {  
    float distanceSquared = (circle_a.GetPosition().x - circle_b.GetPosition().x) * (circle_a.GetPosition().x - circle_b.GetPosition().x) +  
					(circle_a.GetPosition().y - circle_b.GetPosition().y) * (circle_a.GetPosition().y - circle_b.GetPosition().y);  
    float radiusSum = circle_a.GetRadius() + circle_b.GetRadius();  
    return distanceSquared <= (radiusSum * radiusSum);  
    }  

    bool CollisionMgr::CheckBoxAndCircleCollision(const BoxCollider2D& box, const CircleCollider2D& circle)  
    {  
    float closestX = std::fmax(box.GetPosition().x - box.GetSize().x / 2, std::fmin(circle.GetPosition().x, box.GetPosition().x + box.GetSize().x / 2));  
    float closestY = std::fmax(box.GetPosition().y - box.GetSize().y / 2, std::fmin(circle.GetPosition().y, box.GetPosition().y + box.GetSize().y / 2));  

    float distanceX = circle.GetPosition().x - closestX;  
    float distanceY = circle.GetPosition().y - closestY;  

    return (distanceX * distanceX + distanceY * distanceY) <= (circle.GetRadius() * circle.GetRadius());  
    }  

    void CollisionMgr::HandleCollisionResponse(Collider2D* coll_a, Collider2D* coll_b)  
    {  
    if (coll_a->IsTrigger()|| coll_b->IsTrigger()){return;}  

    Rigidbody2D* rb_a = coll_a->gameObject->GetComponent<Rigidbody2D>();  
    Rigidbody2D* rb_b = coll_b->gameObject->GetComponent<Rigidbody2D>();  

    // 少なくとも1つのオブジェクトが動的であることを確認（Rigidbodyを持ち、かつ運動学的でない）  
    if ((rb_a == nullptr || rb_a->IsKinematic()) && (rb_b == nullptr || rb_b->IsKinematic()))  
					return;  

    // 衝突法線と貫通深度を計算  
    XMFLOAT2 normal{};  
    float penetration;  
    if (!ComputeCollisionManifold(coll_a, coll_b, normal, penetration))  
					return; // 計算できない場合は直接戻る  

    // 位置補正  
    float invMassA = (rb_a && !rb_a->IsKinematic()) ? 1.0f / rb_a->GetMass() : 0.0f;  
    float invMassB = (rb_b && !rb_b->IsKinematic()) ? 1.0f / rb_b->GetMass() : 0.0f;  
    float totalInverseMass = invMassA + invMassB;  

    // ゼロ除算を防ぐ  
    if (totalInverseMass <= 0.0f)  
					return;  

    // 補正量を計算  
    XMFLOAT2 correction = normal * (penetration / totalInverseMass);  

    // 位置補正を適用  
    if (invMassA > 0.0f)  
					rb_a->gameObject->transform.position -= correction * invMassA;  
    if (invMassB > 0.0f)  
					rb_b->gameObject->transform.position += correction * invMassB;  

    // 相対速度を計算  
    XMFLOAT2 velocityA = (rb_a && !rb_a->IsKinematic()) ? rb_a->GetVelocity() : XMFLOAT2(0.0f, 0.0f);  
    XMFLOAT2 velocityB = (rb_b && !rb_b->IsKinematic()) ? rb_b->GetVelocity() : XMFLOAT2(0.0f, 0.0f);  
    XMFLOAT2 relativeVelocity = velocityA - velocityB;  

    // 法線方向の速度成分を計算  
    float velocityAlongNormal = Dot(relativeVelocity, normal);  

    // 弾性衝撃量を計算（法線方向のみ）  
    float restitution = 0.0f; // 弾性係数、必要に応じて設定  
    float impulseScalar = -(1 + restitution) * velocityAlongNormal / totalInverseMass;  
    XMFLOAT2 impulse = normal * impulseScalar;  

    // 衝撃量を適用  
    if (invMassA > 0.0f)  
    {  
					// 速度を分解  
					XMFLOAT2 vA = velocityA;  
					float vnA = Dot(vA, normal);  
					XMFLOAT2 vnA_vec = normal * vnA;  
					XMFLOAT2 vtA = vA - vnA_vec;  

					// 法線方向の速度を更新  
					vnA_vec += impulse * invMassA;  

					// 新しい速度を合成  
					XMFLOAT2 newVelocityA = vtA + vnA_vec;  
					rb_a->SetVelocity(newVelocityA);  
    }  
    if (invMassB > 0.0f)  
    {  
					// 速度を分解  
					XMFLOAT2 vB = velocityB;  
					float vnB = Dot(vB, normal);  
					XMFLOAT2 vnB_vec = normal * vnB;  
					XMFLOAT2 vtB = vB - vnB_vec;  

					// 法線方向の速度を更新  
					vnB_vec -= impulse * invMassB;  

					// 新しい速度を合成  
					XMFLOAT2 newVelocityB = vtB + vnB_vec;  
					rb_b->SetVelocity(newVelocityB);  
    }  
    }  

    bool CollisionMgr::ComputeCollisionManifold(Collider2D* coll_a, Collider2D* coll_b, XMFLOAT2& normal, float& penetration)  
    {  
    // 衝突体のタイプを取得  
    E_ColliderType type_a = coll_a->GetColliderType();  
    E_ColliderType type_b = coll_b->GetColliderType();  

    // 円形と円形の衝突  
    if (type_a == E_ColliderType::Circle2D && type_b == E_ColliderType::Circle2D)  
    {  
					CircleCollider2D* circle_a = static_cast<CircleCollider2D*>(coll_a);  
					CircleCollider2D* circle_b = static_cast<CircleCollider2D*>(coll_b);  

					XMFLOAT2 posA = circle_a->GetPosition();  
					XMFLOAT2 posB = circle_b->GetPosition();  

					// AからBへのベクトルを計算  
					XMFLOAT2 delta = { posB.x - posA.x, posB.y - posA.y };  
					float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);  

					// 距離が0の場合、ゼロ除算を避ける  
					if (distance == 0.0f)  
					{  
						normal = { 1.0f, 0.0f }; // デフォルト方向  
						penetration = circle_a->GetRadius() + circle_b->GetRadius();  
					}  
					else  
					{  
						normal = { delta.x / distance, delta.y / distance };  
						penetration = circle_a->GetRadius() + circle_b->GetRadius() - distance;  
					}  

					return penetration > 0;  
    }  

    // 矩形と矩形の衝突  
    else if (type_a == E_ColliderType::Box2D && type_b == E_ColliderType::Box2D)  
    {  
					BoxCollider2D* box_a = static_cast<BoxCollider2D*>(coll_a);  
					BoxCollider2D* box_b = static_cast<BoxCollider2D*>(coll_b);  

					XMFLOAT2 posA = box_a->GetPosition();  
					XMFLOAT2 posB = box_b->GetPosition();  
					XMFLOAT2 halfSizeA = box_a->GetSize() * 0.5f;  
					XMFLOAT2 halfSizeB = box_b->GetSize() * 0.5f;  

					float dx = posB.x - posA.x;  
					float px = (halfSizeA.x + halfSizeB.x) - std::abs(dx);  
					if (px <= 0) return false;  

					float dy = posB.y - posA.y;  
					float py = (halfSizeA.y + halfSizeB.y) - std::abs(dy);  
					if (py <= 0) return false;  

					if (px < py)  
					{  
						normal = { dx < 0 ? -1.0f : 1.0f, 0.0f };  
						penetration = px;  
					}  
					else  
					{  
						normal = { 0.0f, dy < 0 ? -1.0f : 1.0f };  
						penetration = py;  
					}  

					return true;  
    }  

    // 矩形と円形の衝突  
    else if ((type_a == E_ColliderType::Box2D && type_b == E_ColliderType::Circle2D) ||  
       (type_a == E_ColliderType::Circle2D && type_b == E_ColliderType::Box2D))  
    {  
					// box_aが矩形、circle_bが円形であることを確認  
					BoxCollider2D* box;  
					CircleCollider2D* circle;  
					bool swapped = false;  

					if (type_a == E_ColliderType::Box2D)  
					{  
						box = static_cast<BoxCollider2D*>(coll_a);  
						circle = static_cast<CircleCollider2D*>(coll_b);  
					}  
					else  
					{  
						box = static_cast<BoxCollider2D*>(coll_b);  
						circle = static_cast<CircleCollider2D*>(coll_a);  
						swapped = true;  
					}  

					XMFLOAT2 boxPos = box->GetPosition();  
					XMFLOAT2 boxHalfSize = box->GetSize() * 0.5f;  
					XMFLOAT2 circlePos = circle->GetPosition();  

					// 矩形上の最も近い点を見つける  
					XMFLOAT2 closestPoint = {  
						std::clamp(circlePos.x, boxPos.x - boxHalfSize.x, boxPos.x + boxHalfSize.x),  
						std::clamp(circlePos.y, boxPos.y - boxHalfSize.y, boxPos.y + boxHalfSize.y)  
					};  

					// 円の中心から最も近い点へのベクトルを計算  
					XMFLOAT2 delta = {  
						circlePos.x - closestPoint.x,  
						circlePos.y - closestPoint.y  
					};  

					float distanceSquared = delta.x * delta.x + delta.y * delta.y;  

					// 円の中心が矩形の内部にある場合  
					if (distanceSquared == 0.0f)  
					{  
						// 最小貫通方向を見つける  
						XMFLOAT2 depths = {  
							boxHalfSize.x - std::abs(circlePos.x - boxPos.x),  
							boxHalfSize.y - std::abs(circlePos.y - boxPos.y)  
						};  

						if (depths.x < depths.y)  
						{  
							normal = { (circlePos.x > boxPos.x) ? 1.0f : -1.0f, 0.0f };  
							penetration = depths.x;  
						}  
						else  
						{  
							normal = { 0.0f, (circlePos.y > boxPos.y) ? 1.0f : -1.0f };  
							penetration = depths.y;  
						}  
					}  
					else  
					{  
						float distance = std::sqrt(distanceSquared);  
						normal = { delta.x / distance, delta.y / distance };  
						penetration = circle->GetRadius() - distance;  
					}  

					// 衝突体が交換された場合、法線方向を反転する必要がある  
					if (swapped)  
					{  
						normal = { -normal.x, -normal.y };  
					}  

					return penetration > 0;  
    }  

    return false;  
    }  

    void CollisionMgr::ShowDebugColliders()  
    {  
    if (isDebug)  
    {  
					for (Collider2D* collider : colliders2D)  
					{  
						bool isColliding = currentCollisions2D.find(collider) != currentCollisions2D.end();  

						XMFLOAT4 color = isColliding ? XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)  
							: XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);  

						collider->DrawDebug(color);  
					}  
    }  
    }
}