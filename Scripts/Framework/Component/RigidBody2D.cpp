#include "Rigidbody2D.h"  
#include "GameObject.h"  
#include "RigidbodyMgr.h"  
#include "GameTime.h"  

void Rigidbody2D::Start()  
{  
   // Rigidbody2Dを登録する  
   RigidbodyMgr::instance.RegisterRigidbody2D(this);  
}  

void Rigidbody2D::OnDestroy()  
{  
   // Rigidbody2Dを登録解除する  
   RigidbodyMgr::instance.UnregisterRigidbody2D(this);  
}  

Rigidbody2D& Rigidbody2D::AddForce(const XMFLOAT2& force)  
{  
   // 力を加える（運動学的でなく、質量が0以上の場合）  
   if (!isKinematic && mass > 0)  
   {  
       acceleration += force / mass;  
   }  

   return *this;  
}  

Rigidbody2D& Rigidbody2D::AddTorque(float torque)  
{  
   // トルクを加える（運動学的でない場合）  
   if (!isKinematic)  
   {  
       float angularAcceleration = torque / mass;  
       angularVelocity += angularAcceleration;  
   }  

   return *this;  
}  

bool Rigidbody2D::IsKinematic() const  
{  
   // 運動学的かどうかを返す  
   return isKinematic;  
}  

float Rigidbody2D::GetMass() const  
{  
   // 質量を返す  
   return mass;  
}  

XMFLOAT2 Rigidbody2D::GetVelocity() const  
{  
   // 速度を返す  
   return velocity;  
}  

Rigidbody2D& Rigidbody2D::SetVelocity(const XMFLOAT2& v)  
{  
   // 速度を設定する  
   velocity = v;  

   return *this;  
}  

Rigidbody2D& Rigidbody2D::SetDrag(float dg)  
{  
   // 抵抗を設定する  
   drag = dg;  

   return *this;  
}  

Rigidbody2D& Rigidbody2D::SetMass(float m)  
{  
   // 質量を設定する  
   mass = m;  

   return *this;  
}  

Rigidbody2D& Rigidbody2D::SetGravityScale(float g)  
{  
   // 重力スケールを設定する  
   gravityScale = g;  

   return *this;  
}  

Rigidbody2D& Rigidbody2D::SetAngularVelocity(float av)  
{  
   // 角速度を設定する  
   angularVelocity = av;  

   return *this;  
}  

Rigidbody2D& Rigidbody2D::SetAngularDrag(float ad)  
{  
   // 角抵抗を設定する  
   angularDrag = ad;  

   return *this;  
}  

Rigidbody2D& Rigidbody2D::SetIsKinematic(bool flag)  
{  
   // 運動学的かどうかを設定する  
   isKinematic = flag;  

   return *this;  
}  

float Rigidbody2D::GetGravityScale() const  
{  
   // 重力スケールを返す  
   return gravityScale;  
}  

void Rigidbody2D::MovePosition(const XMFLOAT2& newPosition)  
{  
   // 新しい位置に移動する  
   if (isKinematic)  
       gameObject->transform.position = newPosition;  
   else  
   {  
       if (std::abs(newPosition.x - gameObject->transform.position.x) > 0.001f ||  
           std::abs(newPosition.y - gameObject->transform.position.y) > 0.001f)  
       {  
           targetPosition = newPosition;  
           shouldMovePosition = true;  
       }  
   }  
}  

void Rigidbody2D::PhysicsUpdate()  
{  
   // 物理更新処理  
   if (isKinematic) return;  

   // 重力を加える  
   acceleration += gravity * gravityScale;  

   // 速度を更新する  
   velocity += acceleration * Time::GetFixedDeltaTime();  

   // 抵抗を適用する  
   velocity *= (1.0f - drag);  

   if (shouldMovePosition)  
   {  
       // 目標位置にスムーズに移動する  
       XMFLOAT2 currentPosition = ToXMFLOAT2(gameObject->transform.position);  

       XMFLOAT2 expectedPosByPhysics = currentPosition + velocity * Time::GetFixedDeltaTime();  

       XMFLOAT2 targetOffset = targetPosition - currentPosition;  

       XMFLOAT2 smoothOffset = targetOffset * Time::GetFixedDeltaTime();  

       XMFLOAT2 finalPos = expectedPosByPhysics + smoothOffset;  

       gameObject->transform.position = finalPos;  
       shouldMovePosition = false;  
   }  
   else  
   {  
       // 通常の物理移動  
       gameObject->transform.position += velocity * Time::GetFixedDeltaTime();  
   }  

   // 回転を更新する  
   gameObject->transform.rotation += angularVelocity * Time::GetFixedDeltaTime();  
   angularVelocity *= (1.0f - angularDrag);  

   // 加速度をリセットする  
   acceleration = XMFLOAT2(0.0f, 0.0f);  
}