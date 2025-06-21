#pragma once
#include "Component.h"
#include "main.h"
#include <unordered_map>
#include "E_Tag.h"

class GameObject;

enum class E_HitBoxID
{
	Test_01,
	Test_02,
	Test_03,
	Test_04,
};

class HitBoxMgr2D :public Component
{
public:
	void Start()override;

	HitBoxMgr2D& AddHitBox(E_HitBoxID type, XMFLOAT2 size, XMFLOAT2 offset = XMFLOAT2(0.0f, 0.0f));
	HitBoxMgr2D& AddHitBox(E_HitBoxID type, float radius, XMFLOAT2 offset = XMFLOAT2(0.0f, 0.0f));

	HitBoxMgr2D& ActiveHitBox(E_HitBoxID type);
	HitBoxMgr2D& DisableHitBox(E_HitBoxID type);

private:
	std::unordered_map<E_HitBoxID, GameObject*> hitBoxes;
};