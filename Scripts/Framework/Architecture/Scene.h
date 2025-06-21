#pragma once

#include <vector>
#include <memory>
#include <string>
#include <stdexcept>
#include "E_SceneType.h"

class Camera;
class GameObject;

class Scene
{
public:
virtual ~Scene() = default;

// ゲームオブジェクトを追加する
GameObject& AddGameObject(GameObject* obj);

// シーンを初期化する
virtual void Init() = 0;

// シーンの終了処理を行う
virtual void Uninit() {}

// シーンの開始処理を行う
void Start();

// 固定更新処理を行う
void FixedUpdate();

// 更新処理を行う
void Update();

// 遅延更新処理を行う
void LateUpdate();

// 描画処理を行う
void Draw();

// 破棄処理を行う
void HandleDestroy();

// 全てのオブジェクトを破棄する
void DestroyAll();

// メインカメラを設定する
void SetMainCamera();

// メインカメラを取得する
Camera* GetMainCamera() const;

// ゲームオブジェクトのリストを取得する
const std::vector<std::unique_ptr<GameObject>>& GetGameObjectList() const;
private:
std::vector<std::unique_ptr<GameObject>> gameObjects;
GameObject* mainCamera = nullptr;
};
