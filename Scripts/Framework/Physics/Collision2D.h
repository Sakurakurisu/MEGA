#pragma once

#include "GameObject.h"
#include "Collider2D.h"
#include "Rigidbody2D.h"

class Transform;

class Collision2D
{
public:
	GameObject& gameObject;
	Transform& transform;
	Rigidbody2D* rigidBody = nullptr;
	Collider2D& collider;

	XMFLOAT2 normal;
	float penetration;
	XMFLOAT2 contactPoint;

	Collision2D(Collider2D& coll, const XMFLOAT2& norm = XMFLOAT2(0, 0),
				float penet = 0.0f, const XMFLOAT2& contact = XMFLOAT2(0, 0))
		: gameObject(*(coll.gameObject)),
		transform(gameObject.transform),
		collider(coll),
		rigidBody(gameObject.GetComponent<Rigidbody2D>()),
		normal(norm),
		penetration(penet),
		contactPoint(contact) {}
};
