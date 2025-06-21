#pragma once

#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include "Component.h"
#include "E_Tag.h"
#include "Transform.h"
#include "SceneMgr.h"
#include "E_Layer.h"

class Collision2D;
class Scene;

class GameObject
{
public:
Transform transform = { XMFLOAT3(0.0f, 0.0f, 0.0f) ,XMFLOAT3(1.0f, 1.0f, 1.0f) ,0.0f };

E_Tag tag = E_Tag::Default;
// タグを設定する
GameObject& SetTag(E_Tag tag);

E_Layer layer = E_Layer::Default;
// レイヤーを設定する
GameObject& SetLayer(E_Layer newLayer);

std::string name = "GameObject";
// 名前を設定する
GameObject& SetName(const std::string& name);

// オブジェクトのIDを取得する
uint32_t GetID() const;
__declspec(property(get = GetID)) uint32_t id;

Scene* scene = nullptr;

virtual ~GameObject() = default;

GameObject();
GameObject(const std::string& name);

// ワールド座標を設定する
GameObject& SetPosition(const XMFLOAT3& position);

// ローカル座標を設定する
GameObject& SetLocalPosition(const XMFLOAT3& position);

// スケールを設定する
GameObject& SetScale(const XMFLOAT3& scale);

// 回転を設定する
GameObject& SetRotate(float rotate);

// 親オブジェクトを設定する
GameObject& SetParent(GameObject* parent);

// オブジェクトのアクティブ状態を設定する
GameObject& SetActive(bool flag);
// オブジェクトがアクティブかどうかを確認する
bool IsActive() const;
__declspec(property(get = IsActive, put = SetActive)) bool activeSelf;

// オブジェクトが破棄待ち状態かどうかを確認する
bool IsPendingDestroy() const;

template<typename T>
T& AddComponent()
{
	static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
	auto component = std::make_unique<T>();

	T& ref = *component;
	ref.gameObject = this;
	components[typeid(T)] = std::move(component);

	ref.Init();

	return ref;
}

template<typename T>
T* GetComponent()
{
	if (auto it = components.find(typeid(T)); it != components.end())
	{
		return dynamic_cast<T*>(it->second.get());
	}
	return nullptr;
}

// オブジェクトが有効化されたときに呼び出される
void OnEnable();

// 初期化処理を行う
void Start();

// 固定更新処理を行う
void FixedUpdate();

// 更新処理を行う
void Update();

// 遅延更新処理を行う
void LateUpdate();

// 2D衝突が開始したときに呼び出される
void OnCollisionEnter2D(Collision2D& other);

// 2D衝突が終了したときに呼び出される
void OnCollisionExit2D(Collision2D& other);

// 2D衝突が継続しているときに呼び出される
void OnCollisionStay2D(Collision2D& other);

// 2Dトリガーが開始したときに呼び出される
void OnTriggerEnter2D(Collision2D& other);

// 2Dトリガーが終了したときに呼び出される
void OnTriggerExit2D(Collision2D& other);

// 2Dトリガーが継続しているときに呼び出される
void OnTriggerStay2D(Collision2D& other);

// オブジェクトが無効化されたときに呼び出される
void OnDisable();

// オブジェクトが破棄されるときに呼び出される
void OnDestroy();

template <typename T = GameObject>
// 新しいGameObjectを生成する
static GameObject* Instantiate()
{
	static_assert(std::is_base_of_v<GameObject, T>, "T must be derived from GameObject");

	Scene* activeScene = SceneMgr::instance.GetActiveScene();
	if (!activeScene)
		throw std::runtime_error("No active scene. Cannot instantiate GameObject.");

	auto obj = new T();
	obj->scene = activeScene;

	return dynamic_cast<T*>(&activeScene->AddGameObject(obj));
}

template <typename T = GameObject>
// 指定された位置と回転で新しいGameObjectを生成する
static GameObject* Instantiate(const XMFLOAT3& position, float rotation)
{
	static_assert(std::is_base_of_v<GameObject, T>, "T must be derived from GameObject");

	Scene* activeScene = SceneMgr::instance.GetActiveScene();
	if (!activeScene)
		throw std::runtime_error("No active scene. Cannot instantiate GameObject.");

	auto obj = new T();
	obj->scene = activeScene;

	obj->SetPosition(position);
	obj->SetRotate(rotation);

	return dynamic_cast<T*>(&activeScene->AddGameObject(obj));
}

// 指定されたGameObjectを破棄する
void Destroy(GameObject* obj);

// 名前でGameObjectを検索する
static GameObject* Find(const std::string& name);
// タグでGameObjectを検索する
static GameObject* FindWithTag(E_Tag tag);
// タグで最初に見つかったGameObjectを検索する
static GameObject* FindGameObjectWithTag(E_Tag tag); 
// タグで全てのGameObjectを検索する
static std::vector<GameObject*> FindGameObjectsWithTag(E_Tag tag);

private:
std::unordered_map<std::type_index, std::unique_ptr<Component>> components;

static uint32_t nextID;

uint32_t _id = 0;

bool _activeSelf = true;

bool isPendingDestroy = false;

virtual void LoadComponents() {}
};
