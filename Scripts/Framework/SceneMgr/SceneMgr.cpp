#include "SceneMgr.h"

#include "TestGameScene.h"
#include "DoTween.h"
#include "RigidbodyMgr.h"
#include "CollisionMgr.h"
#include "TimerMgr.h"

void SceneMgr::Init()
{
	activeScene = CreateScene(activeSceneType);
	activeScene->Init();
	activeScene->Start();
}

void SceneMgr::Uninit()
{
	if (activeScene)
	{
		DOTween::KillAll();
		TimerMgr::instance.ClearAllTimer();
		DOTween::KillAll();
		activeScene->DestroyAll();
		activeScene->Uninit();
		activeScene.reset();
	}
}

void SceneMgr::Start()
{
	if (activeScene)
		activeScene->Start();
}

void SceneMgr::FixedUpdate()
{
	if (activeScene)
		activeScene->FixedUpdate();  // 現在のシーンで物理演算を実行
}

void SceneMgr::Update()
{
	if (activeScene)
		activeScene->Update();  // 現在のシーンを更新
}

void SceneMgr::LateUpdate()
{
	if (activeScene)
		activeScene->LateUpdate();  // 現在のシーンで遅延更新処理を実行
}

void SceneMgr::HandleDestroy()
{
	if (activeScene)
		activeScene->HandleDestroy();  // 現在のシーンで破棄処理を実行
}

void SceneMgr::Draw()
{
	if (activeScene)
		activeScene->Draw();  // 現在のシーンを描画
}

void SceneMgr::ChangeScene(E_SceneType sceneType)
{
	if (activeSceneType != sceneType)
	{
		isSceneChangePending = true;
		pendingSceneType = sceneType;
	}
}

void SceneMgr::PerformSceneChange()
{
	if (isSceneChangePending)
	{
		Uninit();

		activeSceneType = pendingSceneType;

		activeScene = CreateScene(activeSceneType);
		activeScene->Init();
		activeScene->Start();

		isSceneChangePending = false;
	}
}

std::unique_ptr<Scene> SceneMgr::CreateScene(E_SceneType sceneType)
{
	switch (sceneType)
	{
		case E_SceneType::TestGameScene:
			return std::make_unique<TestGameScene>();
		default:
			throw std::runtime_error("Unknown Scene Type during PerformSceneChange");
	}
}

Camera* SceneMgr::GetMainCamera() const
{
	return activeScene->GetMainCamera();
}

Scene* SceneMgr::GetActiveScene() const
{
	return activeScene.get();
}