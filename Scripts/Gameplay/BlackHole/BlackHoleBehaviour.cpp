#include "BlackHoleBehaviour.h"
#include "EventCenter.h"
#include "GameObject.h"
#include <DirectXMath.h>
#include "SoundMgr.h"

using namespace DirectX;

void BlackHoleBehaviour::Start()
{
	gravitySound = SoundMgr::instance.CreateAudioClip("BlackHole");
}

void BlackHoleBehaviour::OnDestroy()
{
	EventCenter::instance.EventTrigger(E_EventType::BlackHole_Destroy);
	EventCenter::instance.EventTrigger<XMFLOAT3>(E_EventType::BlackHole_Destroy, gameObject->transform.position);
	gravitySound->Stop();
}