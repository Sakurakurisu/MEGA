#pragma once  

#include "BaseManager.h"  
#include "E_SceneType.h"  
#include <memory>  
#include "Scene.h"  

class SceneMgr : public BaseManager<SceneMgr>  
{  
friend class BaseManager<SceneMgr>;  

public:  
// 初期化処理を行います  
void Init();  

// 終了処理を行います  
void Uninit();  

// ゲームの開始処理を行います  
void Start();  

// 固定更新処理を行います  
void FixedUpdate();  

// 更新処理を行います  
void Update();  

// 遅延更新処理を行います  
void LateUpdate();  

// 破棄処理を処理します  
void HandleDestroy();  

// 描画処理を行います  
void Draw();  

// シーンを変更します  
void ChangeScene(E_SceneType sceneType);  

// メインカメラを取得します  
Camera* GetMainCamera() const;  

// アクティブなシーンを取得します  
Scene* GetActiveScene() const;  

// シーン変更を実行します  
void PerformSceneChange();  

private:  
SceneMgr() = default;  
SceneMgr(const SceneMgr&) = delete;  
SceneMgr& operator=(const SceneMgr&) = delete;  

std::unique_ptr<Scene> CreateScene(E_SceneType sceneType);  

std::unique_ptr<Scene> activeScene;  
E_SceneType activeSceneType = E_SceneType::TestGameScene;  
bool isSceneChangePending = false;  
E_SceneType pendingSceneType = E_SceneType::TestGameScene;  
};
