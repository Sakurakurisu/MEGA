#include "SpriteRendererKiller.h"
#include "SpriteRenderer.h"
#include "Collision2D.h"
#include "GameObject.h"
#include "Vector.h"

void SpriteRendererKiller::Start()
{
	camera = SceneMgr::instance.GetMainCamera()->gameObject;
	player = GameObject::Find("player");
}

void SpriteRendererKiller::Update()
{
	if (!player || !camera) return;

	float dis_p = Distance(ToXMFLOAT2(player->transform.position), ToXMFLOAT2(gameObject->transform.position));
	float dis_c = Distance(ToXMFLOAT2(camera->transform.position), ToXMFLOAT2(gameObject->transform.position));

	if (dis_p < 40.0f || dis_c < 40.0f)
	{
		auto sr = gameObject->GetComponent<SpriteRenderer>();
		if (sr)
		{
			sr->enabled = true;
		}

		auto boxCollider = gameObject->GetComponent<BoxCollider2D>();
		if (boxCollider)
		{
			boxCollider->enabled = true;
		}

		auto circleCollider = gameObject->GetComponent<CircleCollider2D>();
		if (circleCollider)
		{
			circleCollider->enabled = true;
		}
	}
	else
	{
		auto sr = gameObject->GetComponent<SpriteRenderer>();
		if (sr)
		{
			sr->enabled = false;
		}

		auto boxCollider = gameObject->GetComponent<BoxCollider2D>();
		if (boxCollider)
		{
			boxCollider->enabled = false;
		}

		auto circleCollider = gameObject->GetComponent<CircleCollider2D>();
		if (circleCollider)
		{
			circleCollider->enabled = false;
		}
	}
}