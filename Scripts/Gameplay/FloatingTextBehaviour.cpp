#include "FloatingTextBehaviour.h"
#include "DoTween.h"
#include "GameObject.h"

void FloatingTextBehaviour::Start()
{
	DOTween::DOMove(gameObject->transform, XMFLOAT3(gameObject->transform.position.x, gameObject->transform.position.y - 1.0f, gameObject->transform.position.y), 1.5f);
	DOTween::DOScale(gameObject->transform, XMFLOAT3(3.0f, 3.0f, 1.0f), 1.5f);
	DOTween::DOFade(gameObject->GetComponent<SpriteRenderer>(), 0.0f, 2.2f)
		->OnComplete([this]()
					 {
						 gameObject->Destroy(gameObject);
					 });
}