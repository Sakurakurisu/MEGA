#include "GroundChecker.h"
#include "GameObject.h"
#include "Collision2D.h"
#include "EventCenter.h"

void GroundChecker::OnTriggerStay2D(Collision2D& other)
{
	if (other.gameObject.layer != groundLayer) return;

	isGrounded = true;
	CheckGroundedStateChange();
}

void GroundChecker::OnTriggerExit2D(Collision2D& other)
{
	if (other.gameObject.layer != groundLayer) return;

	isGrounded = false;
	CheckGroundedStateChange();
}

GroundChecker& GroundChecker::SetGroundLayer(E_Layer layer)
{
	groundLayer = layer;
	return *this;
}

bool GroundChecker::IsGrounded() const
{
	return isGrounded;
}

void GroundChecker::CheckGroundedStateChange()
{
	if (isGrounded == wasGrounded) return;

	wasGrounded = isGrounded;

	EventCenter::instance.EventTrigger(E_EventType::Ground_Check, gameObject->transform.GetParent()->gameObject, isGrounded);
}