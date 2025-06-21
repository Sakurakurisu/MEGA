#include "HitBoxMgr2D.h"
#include "Collider2D.h"
#include "Debug.h"
#include "GameObject.h"
#include "HitEffect.h"

void HitBoxMgr2D::Start()
{
	for (auto obj = hitBoxes.begin(); obj != hitBoxes.end(); ++obj)
	{
		this->gameObject->scene->AddGameObject(obj->second);
	}
}

HitBoxMgr2D& HitBoxMgr2D::AddHitBox(E_HitBoxID type, XMFLOAT2 size, XMFLOAT2 offset)
{
	GameObject* obj = new GameObject("hitBox");

	obj->SetParent(gameObject)
		.SetLayer(E_Layer::HitBox)
		.SetLocalPosition(XMFLOAT3(offset.x, offset.y, 0.0f))
		.SetActive(false);

	obj->AddComponent<BoxCollider2D>()
		.SetSize(size.x, size.y)
		.SetTrigger(true);

	obj->AddComponent<HitEffect>();

	hitBoxes[type] = obj;

	return *this;
}

HitBoxMgr2D& HitBoxMgr2D::AddHitBox(E_HitBoxID type, float radius, XMFLOAT2 offset)
{
	GameObject* obj = new GameObject("hitBox");

	obj->SetParent(gameObject)
		.SetLayer(E_Layer::HitBox)
		.SetLocalPosition(XMFLOAT3(offset.x, offset.y, 0.0f))
		.SetActive(false);

	obj->AddComponent<CircleCollider2D>()
		.SetRadius(radius)
		.SetTrigger(true);

	obj->AddComponent<HitEffect>();

	hitBoxes[type] = obj;

	return *this;
}

HitBoxMgr2D& HitBoxMgr2D::ActiveHitBox(E_HitBoxID type)
{
	hitBoxes[type]->SetActive(true);

	return *this;
}

HitBoxMgr2D& HitBoxMgr2D::DisableHitBox(E_HitBoxID type)
{
	hitBoxes[type]->SetActive(false);

	return *this;
}