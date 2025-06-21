#include "RigidbodyMgr.h"
#include "Rigidbody2D.h"

void RigidbodyMgr::RegisterRigidbody2D(Rigidbody2D* rb)
{
	rigidbodies2D.push_back(rb);
}

void RigidbodyMgr::UnregisterRigidbody2D(Rigidbody2D* rb)
{
	auto it = std::find(rigidbodies2D.begin(), rigidbodies2D.end(), rb);
	if (it != rigidbodies2D.end())
	{
		rigidbodies2D.erase(it);
	}
}

void RigidbodyMgr::ClearAllRigidbody2D()
{
	rigidbodies2D.clear();
}

void RigidbodyMgr::UpdateRigidbody()
{
	for (auto& rb : rigidbodies2D)
	{
		rb->PhysicsUpdate();
	}
}