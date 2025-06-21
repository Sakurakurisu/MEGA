#include "AttackBehaviour.h"
#include "AttackInfo.h"
#include "EventCenter.h"
#include "GameObject.h"
#include "RigidBody2D.h"
#include "Health.h"

void AttackBehaviour::Start()
{
	rb = gameObject->GetComponent<Rigidbody2D>();
}

void AttackBehaviour::DoAttack()
{
	EventCenter::instance.AddEventListener<AttackInfo>(E_EventType::Attack, this, &AttackBehaviour::OnHit);
}

void AttackBehaviour::Reset()
{
	EventCenter::instance.RemoveEventListener(E_EventType::Attack, this, &AttackBehaviour::OnHit);
}

AttackBehaviour& AttackBehaviour::SetKnockbackForce(float force)
{
	knockbackForce = force;
	return *this;
}

void AttackBehaviour::OnHit(AttackInfo attackInfo)
{
	if (attackInfo.target->tag != E_Tag::Enemy) return;

	attackInfo.target->GetComponent<Health>()
		->BeAttacked(5);

	XMFLOAT2 dir = ToXMFLOAT2(Direction(attackInfo.target->transform.position, attackInfo.attacker->transform.position));
	if (rb)
		rb->AddForce(dir * knockbackForce);
}