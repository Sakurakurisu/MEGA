#pragma once

#include "GameObject.h"

class AttackInfo
{
public:
	GameObject* attacker;
	GameObject* target;
	int damage;

	AttackInfo(GameObject* attacker, GameObject* target, int damage = 0) : attacker(attacker), target(target), damage(damage) {}
};