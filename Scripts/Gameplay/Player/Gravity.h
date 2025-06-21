#pragma once

#include "Component.h"
#include "E_Tag.h"
#include <vector>

class Gravity : public Component
{
public:
	Gravity() = default;

	Gravity& SetGravity(float gravity);

	Gravity& AddAffectedTag(E_Tag tag);

	Gravity& RemoveAffectedTag(E_Tag tag);

	Gravity& ClearAffectedTags();

	Gravity& SetRange(float range);

	float GetRange() const;

	bool IsTagAffected(E_Tag tag) const;

	void OnEnable() override;

	void OnTriggerStay2D(Collision2D& other) override;

private:
	float gravity = 10.0f;
	float affectedRange = 10.0f;
	std::vector<E_Tag> affectedTags;
};