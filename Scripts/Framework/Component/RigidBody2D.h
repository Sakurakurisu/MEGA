#pragma once

#include "Component.h"
#include "Vector.h"
#include <DirectXMath.h>

using namespace DirectX;

class Rigidbody2D : public Component
{
public:
Rigidbody2D() = default;
~Rigidbody2D() = default;;

void Start() override;

void OnDestroy() override;

// 物理更新を行います
void PhysicsUpdate();

// 力を加えます
Rigidbody2D& AddForce(const XMFLOAT2& force);

// トルクを加えます
Rigidbody2D& AddTorque(float torque);

// オブジェクトが運動学的かどうかを確認します
bool IsKinematic() const;

// 質量を取得します
float GetMass() const;

// 現在の速度を取得します
XMFLOAT2 GetVelocity() const;

// 速度を設定します
Rigidbody2D& SetVelocity(const XMFLOAT2& v);

// 抗力を設定します
Rigidbody2D& SetDrag(float dg);

// 質量を設定します
Rigidbody2D& SetMass(float m);

// 重力スケールを設定します
Rigidbody2D& SetGravityScale(float g);

// 角速度を設定します
Rigidbody2D& SetAngularVelocity(float av);

// 角抗力を設定します
Rigidbody2D& SetAngularDrag(float ad);

// オブジェクトを運動学的に設定します
Rigidbody2D& SetIsKinematic(bool flag);

// 重力スケールを取得します
float GetGravityScale() const;

// 新しい位置に移動します
void MovePosition(const XMFLOAT2& newPosition);

private:
XMFLOAT2 velocity = XMFLOAT2(0.0f, 0.0f);
XMFLOAT2 acceleration = XMFLOAT2(0.0f, 0.0f);
float drag = 0.05f;
float mass = 1.0f;
float gravityScale = 1.0f;
float angularVelocity = 0.0f;
float angularDrag = 0.05f;
bool isKinematic = false;

const XMFLOAT2 gravity = XMFLOAT2(0.0f, 9.8f);

XMFLOAT2 targetPosition = XMFLOAT2(0.0f, 0.0f);
bool shouldMovePosition = false;
};