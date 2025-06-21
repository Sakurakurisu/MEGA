#include "Gravity.h"
#include "Collision2D.h"
#include "Collider2D.h"
#include "GameObject.h"
#include <DirectXMath.h>
#include "StateMachine.h"
#include "SpriteRenderer.h"

using namespace DirectX;

Gravity& Gravity::SetGravity(float gravity)
{
	this->gravity = gravity;

	return *this;
}

Gravity& Gravity::AddAffectedTag(E_Tag tag)
{
	affectedTags.push_back(tag);

	return *this;
}

Gravity& Gravity::RemoveAffectedTag(E_Tag tag)
{
	affectedTags.erase(std::remove(affectedTags.begin(), affectedTags.end(), tag), affectedTags.end());

	if (tag == E_Tag::Player)
	{
		GameObject::Find("player")->GetComponent<SpriteRenderer>()->GetMaterial().diffuse = { 0.0f,0.0f,0.0f,0.0f };
	}
	return *this;
}

Gravity& Gravity::ClearAffectedTags()
{
	affectedTags.clear();

	return *this;
}

Gravity& Gravity::SetRange(float range)
{
	affectedRange = range;
	return *this;
}

float Gravity::GetRange() const
{
	return affectedRange;
}

bool Gravity::IsTagAffected(E_Tag tag) const
{
	return std::find(affectedTags.begin(), affectedTags.end(), tag) != affectedTags.end();
}

void Gravity::OnEnable()
{
	gameObject->GetComponent<CircleCollider2D>()->SetRadius(affectedRange);
}

void Gravity::OnTriggerStay2D(Collision2D& other)
{
	if (affectedTags.empty() || IsTagAffected(other.gameObject.tag))
	{
		XMFLOAT2 dir = Normalize(ToXMFLOAT2(gameObject->transform.position) - ToXMFLOAT2(other.transform.position));
		
		other.rigidBody->AddForce(dir * gravity);

		if (other.gameObject.tag == E_Tag::Player)
		{
			other.gameObject.GetComponent<SpriteRenderer>()->GetMaterial().diffuse = { 0.7f,0.0f,0.7f,0.0f };
		}
	}
}