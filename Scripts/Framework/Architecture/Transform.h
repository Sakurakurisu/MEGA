#pragma once  

#include <DirectXMath.h>  
#include <vector>  
#include <algorithm>  
#include "Vector.h"  
#include "Component.h"  

using namespace DirectX;  

class GameObject;  

class Transform : public Component  
{  
public:  
   Transform(const XMFLOAT3& pos, const XMFLOAT3& scl, float rot);  
   ~Transform();  

   // 親を設定する  
   void SetParent(Transform* newParent);  
   // 親を取得する  
   Transform* GetParent() const;  

   // ローカル位置を取得する  
   XMFLOAT3 GetLocalPosition() const;  
   // ローカル位置を設定する (2D)  
   void SetLocalPosition(const XMFLOAT2& pos);  
   // ローカル位置を設定する (3D)  
   void SetLocalPosition(const XMFLOAT3& pos);  
   __declspec(property(get = GetLocalPosition, put = SetLocalPosition)) XMFLOAT3 localPosition;  

   // ワールド位置を取得する  
   XMFLOAT3 GetPosition();  
   // ワールド位置を設定する (2D)  
   void SetPosition(const XMFLOAT2& pos);  
   // ワールド位置を設定する (3D)  
   void SetPosition(const XMFLOAT3& pos);  
   __declspec(property(get = GetPosition, put = SetPosition)) XMFLOAT3 position;  

   // ローカル回転を取得する  
   float GetLocalRotation() const;  
   // ローカル回転を設定する  
   void SetLocalRotation(float rot);  
   __declspec(property(get = GetLocalRotation, put = SetLocalRotation)) float localRotation;  

   // ワールド回転を取得する  
   float GetRotation();  
   // ワールド回転を設定する  
   void SetRotation(float rot);  
   __declspec(property(get = GetRotation, put = SetRotation)) float rotation;  

   // ローカルスケールを取得する  
   XMFLOAT3 GetLocalScale();  
   // ローカルスケールを設定する (2D)  
   void SetLocalScale(const XMFLOAT2& scale);  
   // ローカルスケールを設定する (3D)  
   void SetLocalScale(const XMFLOAT3& scale);  
   __declspec(property(get = GetLocalScale, put = SetLocalScale)) XMFLOAT3 localScale;  

   // ワールドスケールを取得する  
   XMFLOAT3 GetScale();  
   // ワールドスケールを設定する (2D)  
   void SetScale(const XMFLOAT2& scale);  
   // ワールドスケールを設定する (3D)  
   void SetScale(const XMFLOAT3& scale);  
   __declspec(property(get = GetScale, put = SetScale)) XMFLOAT3 scale;  

   // ワールド行列を取得する  
   const XMMATRIX& GetWorldMatrix();  

   // 子要素を取得する  
   const std::vector<Transform*>& GetChildren() const;  
   __declspec(property(get = GetChildren)) std::vector<Transform*> children;  

private:  
   XMFLOAT3 _localPosition{ 0.0f,0.0f,0.0f };  
   XMFLOAT3 _localScale{ 1.0f,1.0f,1.0f };  
   float _localRotation = 0.0f;  

   XMFLOAT3 worldPosition{ 0.0f,0.0f,0.0f };  
   XMFLOAT3 worldScale{ 1.0f,1.0f,1.0f };  
   float worldRotation = 0.0f;  

   std::vector<Transform*> _children;  

   XMMATRIX worldMatrix = XMMatrixIdentity();  

   Transform* parent = nullptr;  

   bool isDirty = true;  
   bool isUpdating = false;  

   void MarkDirty();  
   void UpdateWorldMatrix();  
   void UpdateWorldTransform();  
   void UpdateLocalTransform();  
};
