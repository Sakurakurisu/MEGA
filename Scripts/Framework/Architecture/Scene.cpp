#include "Scene.h"  
#include "GameObject.h"  
#include "RenderSystem.h"  
#include "Camera.h"  
#include "CollisionMgr.h"  
#include "RigidbodyMgr.h"  
#include "SpriteRenderer.h"  
#include "SceneMgr.h"  

GameObject& Scene::AddGameObject(GameObject* obj)  
{  
   // ゲームオブジェクトをシーンに追加します  
   obj->scene = this;  
   gameObjects.push_back(std::unique_ptr<GameObject>(obj));  

   return *gameObjects.back();  
}  

void Scene::Start()  
{  
   // 各ゲームオブジェクトのStartメソッドを呼び出します  
   for (size_t i = 0; i < gameObjects.size(); ++i)  
   {  
       if (gameObjects[i]->IsActive())  
       {  
           gameObjects[i]->Start();  
       }  
   }  
}  

void Scene::FixedUpdate()  
{  
   // 物理演算と衝突判定を更新します  
   RigidbodyMgr::instance.UpdateRigidbody();  
   CollisionMgr::instance.CheckCollisions();  

   // 各ゲームオブジェクトのFixedUpdateメソッドを呼び出します  
   for (size_t i = 0; i < gameObjects.size(); ++i)  
   {  
       if (gameObjects[i]->IsActive())  
           gameObjects[i]->FixedUpdate();  
   }  
}  

void Scene::Update()  
{  
   // 各ゲームオブジェクトのUpdateメソッドを呼び出します  
   for (size_t i = 0; i < gameObjects.size(); ++i)  
   {  
       if (gameObjects[i]->IsActive())  
           gameObjects[i]->Update();  
   }  
}  

void Scene::LateUpdate()  
{  
   // 各ゲームオブジェクトのLateUpdateメソッドを呼び出します  
   for (size_t i = 0; i < gameObjects.size(); ++i)  
   {  
       if (gameObjects[i]->IsActive())  
           gameObjects[i]->LateUpdate();  
   }  

   // デバッグ用のコライダーを表示します  
   CollisionMgr::instance.ShowDebugColliders();  
}  

void Scene::Draw()  
{  
   // メインカメラを取得し、描画を行います  
   Camera* camera = GetMainCamera();  
   if (!camera) return;  
   RenderSystem::instance.Render(camera);  
   RenderSystem::instance.RenderText(camera);  
}  

void Scene::HandleDestroy()  
{  
   // 削除予定のゲームオブジェクトを処理します  
   size_t i = 0;  
   while (i < gameObjects.size())  
   {  
       auto& obj = gameObjects[i];  
       if (obj->IsPendingDestroy())  
       {  
           obj->OnDestroy();  

           gameObjects.erase(gameObjects.begin() + i);  
       }  
       else  
           ++i;  
   }  
}  

void Scene::DestroyAll()  
{  
   // 全てのゲームオブジェクトを削除します  
   for (auto& obj : gameObjects)  
   {  
       obj->Destroy(obj.get());  
   }  

   HandleDestroy();  

   mainCamera = nullptr;  
}  

//void Scene::LoadScene(E_SceneType scene)  
//{  
//    // シーンを切り替える処理  
//    nextScene = scene;  
//    changeScene = true;  
//}  

void Scene::SetMainCamera()  
{  
   // メインカメラを設定します  
   mainCamera = new GameObject("Main Camera");  

   mainCamera->AddComponent<Camera>()  
       .SetOrthographic(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 10.0f);  

   AddGameObject(mainCamera);  
}  

Camera* Scene::GetMainCamera() const  
{  
   // メインカメラを取得します  
   return mainCamera ? mainCamera->GetComponent<Camera>() : nullptr;  
}  

const std::vector<std::unique_ptr<GameObject>>& Scene::GetGameObjectList() const  
{  
   // ゲームオブジェクトのリストを取得します  
   return gameObjects;  
}