#include "Component.h"
#include "GameObject.h"

void Component::SetEnabled(bool flag)
{
	if (_enabled == flag) return;

	_enabled = flag;
	if (isInitialized)
		_enabled ? OnEnable() : OnDisable();
}

bool Component::IsEnabled() const
{
	return _enabled;
}

void Component::Destroy(GameObject* obj)
{
	obj->Destroy(obj);
}