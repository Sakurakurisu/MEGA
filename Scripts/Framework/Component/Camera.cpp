#include "Camera.h"
#include "GameObject.h"

void Camera::Update()
{
	UpdateViewMatrix();
}

Camera& Camera::SetOrthographic(float width, float height, float nearZ, float farZ)
{
	this->width = width / 100.0f; // ピクセル→ワールド単位に変換
	this->height = height / 100.0f;
	this->nearZ = nearZ;
	this->farZ = farZ;
	UpdateProjectionMatrix(); // 射影行列を再計算

	return *this;
}

Camera& Camera::SetSize(float width, float height)
{
	this->width = width;
	this->height = height;
	UpdateProjectionMatrix(); // サイズ変更後に射影行列を更新

	return *this;
}

Camera& Camera::SetNearZ(float nearZ)
{
	this->nearZ = nearZ;
	UpdateProjectionMatrix();

	return *this;
}

Camera& Camera::SetFarZ(float farZ)
{
	this->farZ = farZ;
	UpdateProjectionMatrix();

	return *this;
}

const XMMATRIX& Camera::GetViewMatrix() const
{
	return viewMatrix;
}

const XMMATRIX& Camera::GetProjectionMatrix() const
{
	return projectionMatrix;
}

float Camera::GetPixelsPerUnit() const
{
	return pixelsPerUnit; // 1ユニットあたりのピクセル数（ズーム制御などに利用）
}

void Camera::ApplyViewProjection() const
{
	// ビュー行列と射影行列をエフェクトにセット（描画前に呼び出す）
	BasicEffect::instance.SetViewMatrix(viewMatrix);
	BasicEffect::instance.SetProjectionMatrix(projectionMatrix);
}

void Camera::UpdateViewMatrix()
{
	// カメラ位置を元にビュー行列を作成
	XMFLOAT3 position = gameObject->transform.position;

	XMVECTOR posVec = XMVectorSet(position.x, position.y, position.z, 1.0f);         // 視点位置
	XMVECTOR lookAtVec = XMVectorSet(position.x, position.y, position.z + 1.0f, 1.0f); // 前方を見る
	XMVECTOR upVec = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);                              // 上方向

	viewMatrix = XMMatrixLookAtLH(posVec, lookAtVec, upVec); // 左手座標系ビュー行列
}

void Camera::UpdateProjectionMatrix()
{
	// 正射影行列の生成（高さを負にしてY軸反転に対応）
	projectionMatrix = XMMatrixOrthographicLH(width, -height, nearZ, farZ);
}