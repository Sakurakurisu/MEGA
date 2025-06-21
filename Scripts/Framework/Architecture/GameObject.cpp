#include "GameObject.h"

uint32_t GameObject::nextID = 1000;

GameObject& GameObject::SetName(const std::string& name)
{
this->name = name;
return *this;
}

uint32_t GameObject::GetID() const
{
// オブジェクトの一意なIDを取得します
return _id;
}

GameObject::GameObject()
{
transform.gameObject = this;
}

GameObject::GameObject(const std::string& n) : name(n)
{
transform.gameObject = this;
}

GameObject& GameObject::SetTag(E_Tag tag)
{
this->tag = tag;

return *this;
}

GameObject& GameObject::SetLayer(E_Layer newLayer)
{
layer = newLayer;
return *this;
}

GameObject& GameObject::SetPosition(const XMFLOAT3& position)
{
// オブジェクトのワールド座標を設定します
transform.position = position;

return *this;
}

GameObject& GameObject::SetLocalPosition(const XMFLOAT3& position)
{
// オブジェクトのローカル座標を設定します
transform.localPosition = position;

return *this;
}

GameObject& GameObject::SetScale(const XMFLOAT3& scale)
{
// オブジェクトのスケールを設定します
transform.scale = scale;

return *this;
}

GameObject& GameObject::SetRotate(float rotation)
{
// オブジェクトの回転を設定します
transform.rotation = rotation;

return *this;
}

GameObject& GameObject::SetParent(GameObject* parent)
{
// オブジェクトの親を設定します
transform.SetParent(&parent->transform);

return *this;
}

GameObject& GameObject::SetActive(bool flag)
{
if (_activeSelf == flag) return *this;

_activeSelf = flag;

// オブジェクトが有効化または無効化されたときの処理
_activeSelf ? OnEnable() : OnDisable();

return *this;
}

bool GameObject::IsActive() const
{
// オブジェクトが有効かどうかを確認します
return _activeSelf;
}

bool GameObject::IsPendingDestroy() const
{
// オブジェクトが破棄待ち状態かどうかを確認します
return isPendingDestroy;
}

void GameObject::OnEnable()
{
// オブジェクトが有効化されたときに呼び出される処理
for (auto& pair : components)
{
	if (pair.second->enabled && pair.second->isInitialized)
		pair.second->OnEnable();
}
}

void GameObject::Start()
{
// オブジェクトの初期化処理
for (auto& pair : components)
{
	if (pair.second->enabled && !pair.second->startCalled)
	{
		pair.second->OnEnable();
	}
}

for (auto& pair : components)
{
	if (pair.second->enabled && !pair.second->startCalled)
	{
		pair.second->Start();
		pair.second->startCalled = true;
		pair.second->isInitialized = true;
	}
}
}

void GameObject::FixedUpdate()
{
// 固定更新処理
for (auto& pair : components)
{
	if (pair.second->enabled)
		pair.second->FixedUpdate();
}
}

void GameObject::Update()
{
// 毎フレーム更新処理
for (auto& pair : components)
{
	if (pair.second->enabled)
		pair.second->Update();
}
}

void GameObject::LateUpdate()
{
// 毎フレームの後処理
for (auto& pair : components)
{
	if (pair.second->enabled)
		pair.second->LateUpdate();
}
}

void GameObject::OnCollisionEnter2D(Collision2D& other)
{
// 2D衝突が開始したときの処理
for (auto& pair : components)
{
	if (pair.second->enabled)
		pair.second->OnCollisionEnter2D(other);
}
}

void GameObject::OnCollisionExit2D(Collision2D& other)
{
// 2D衝突が終了したときの処理
for (auto& pair : components)
{
	if (pair.second->enabled)
		pair.second->OnCollisionExit2D(other);
}
}

void GameObject::OnCollisionStay2D(Collision2D& other)
{
// 2D衝突が継続しているときの処理
for (auto& pair : components)
{
	if (pair.second->enabled)
		pair.second->OnCollisionStay2D(other);
}
}

void GameObject::OnTriggerEnter2D(Collision2D& other)
{
// 2Dトリガーが開始したときの処理
for (auto& pair : components)
{
	if (pair.second->enabled)
		pair.second->OnTriggerEnter2D(other);
}
}

void GameObject::OnTriggerExit2D(Collision2D& other)
{
// 2Dトリガーが終了したときの処理
for (auto& pair : components)
{
	if (pair.second->enabled)
		pair.second->OnTriggerExit2D(other);
}
}

void GameObject::OnTriggerStay2D(Collision2D& other)
{
// 2Dトリガーが継続しているときの処理
for (auto& pair : components)
{
	if (pair.second->enabled)
		pair.second->OnTriggerStay2D(other);
}
}

void GameObject::OnDisable()
{
// オブジェクトが無効化されたときに呼び出される処理
for (auto& pair : components)
{
	if (pair.second->enabled && pair.second->isInitialized)
		pair.second->OnDisable();
}
}

void GameObject::OnDestroy()
{
// オブジェクトが破棄されるときに呼び出される処理
for (auto& pair : components)
{
	if (pair.second->enabled)
		pair.second->OnDestroy();
}
}

void GameObject::Destroy(GameObject* obj)
{
// オブジェクトを破棄します
obj->transform.SetParent(nullptr);
obj->isPendingDestroy = true;
for (auto* child : obj->transform.children)
{
	child->gameObject->Destroy(child->gameObject);
}
}

GameObject* GameObject::Find(const std::string& name)
{
// 名前でオブジェクトを検索します
Scene* activeScene = SceneMgr::instance.GetActiveScene();
if (!activeScene)
	return nullptr;

for (const auto& obj : activeScene->GetGameObjectList())
{
	if (obj->name == name && !obj->IsPendingDestroy())
		return obj.get();
}
return nullptr;
}

GameObject* GameObject::FindWithTag(E_Tag tag)
{
// タグでオブジェクトを検索します
Scene* activeScene = SceneMgr::instance.GetActiveScene();
if (!activeScene)
	return nullptr;

for (const auto& obj : activeScene->GetGameObjectList())
{
	if (obj->tag == tag && !obj->IsPendingDestroy())
		return obj.get();
}
return nullptr;
}

GameObject* GameObject::FindGameObjectWithTag(E_Tag tag)
{
// タグで最初に見つかったオブジェクトを返します
return FindWithTag(tag);
}

std::vector<GameObject*> GameObject::FindGameObjectsWithTag(E_Tag tag)
{
// タグで一致するすべてのオブジェクトを検索します
std::vector<GameObject*> result;
Scene* activeScene = SceneMgr::instance.GetActiveScene();
if (!activeScene)
	return result;

for (const auto& obj : activeScene->GetGameObjectList())
{
	if (obj->tag == tag && !obj->IsPendingDestroy())
		result.push_back(obj.get());
}
return result;
}