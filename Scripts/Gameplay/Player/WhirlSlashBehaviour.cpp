#include "WhirlSlashBehaviour.h"
#include "AttackInfo.h"
#include "EventCenter.h"
#include "GameObject.h"
#include "Health.h"
#include "StateMachine.h"
#include"SoundMgr.h"
#include"AudioClip.h"
#include "Health.h"

bool WhirlSlashBehaviour::ShouldJump() const
{
	return shouldJump;
}

void WhirlSlashBehaviour::DoWhirlSlash()
{
	EventCenter::instance.AddEventListener<AttackInfo>(E_EventType::Attack, this, &WhirlSlashBehaviour::OnHit);
	gameObject->GetComponent<Health>()->SetActiveInvincivle(0.6f);
}

void WhirlSlashBehaviour::Reset()
{
	shouldJump = false;
	EventCenter::instance.RemoveEventListener(E_EventType::Attack, this, &WhirlSlashBehaviour::OnHit);
}

void WhirlSlashBehaviour::OnHit(AttackInfo attackInfo)
{
	if (attackInfo.target->tag != E_Tag::Enemy) return;
	if (attackInfo.target->tag != E_Tag::Enemy) return;
	Health* health = attackInfo.target->GetComponent<Health>();
	health->BeAttacked(10);

	if (health->IsDie())
	{
		shouldJump = true;
	}
	else
	{
		attackInfo.target->GetComponent<StateMachine>()->ChangeState(E_StateType::Enemy_Hit);
	}
}