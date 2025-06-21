#pragma once

#include "Component.h"
#include "E_Layer.h"
#include "Debug.h"

class GroundChecker : public Component
{
public:
	GroundChecker() = default;

	void OnTriggerStay2D(Collision2D& other) override;

	void OnTriggerExit2D(Collision2D& other) override;

	GroundChecker& SetGroundLayer(E_Layer layer);

	bool IsGrounded() const;

private:
	void CheckGroundedStateChange();

	E_Layer groundLayer = E_Layer::Ground;
	bool isGrounded = false;
	bool wasGrounded = false;
};
