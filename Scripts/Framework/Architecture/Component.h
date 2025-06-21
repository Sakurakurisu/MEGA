#pragma once

class GameObject;
class Collision2D;

// コンポーネント基底クラス
class Component
{
public:

	bool startCalled = false;

	bool isInitialized = false;

	void SetEnabled(bool flag);
	bool IsEnabled() const;
	__declspec(property(get = IsEnabled, put = SetEnabled)) bool enabled;

	virtual void Init() {}

	virtual void Awake() {}

	virtual void Start() {}

	virtual void FixedUpdate() {}

	virtual void Update() {}

	virtual void LateUpdate() {}

	virtual void OnCollisionEnter2D(Collision2D& other) {}

	virtual void OnCollisionExit2D(Collision2D& other) {}

	virtual void OnCollisionStay2D(Collision2D& other) {}

	virtual void OnTriggerEnter2D(Collision2D& other) {}

	virtual void OnTriggerExit2D(Collision2D& other) {}

	virtual void OnTriggerStay2D(Collision2D& other) {}

	virtual void OnDestroy() {}

	virtual void OnEnable() {}

	virtual void OnDisable() {}

	void Destroy(GameObject* obj);

	virtual ~Component() = default;

	Component(const Component&) = delete;
	Component& operator=(const Component&) = delete;

	GameObject* gameObject = nullptr;

protected:
	Component() = default;

private:
	bool _enabled = true;
};
