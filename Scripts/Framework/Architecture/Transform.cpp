#include "Transform.h"
#include "GameObject.h"
#include "Vector.h"

Transform::Transform(const XMFLOAT3& pos, const XMFLOAT3& scl, float rot)
	: worldPosition(pos), worldScale(scl), worldRotation(rot) {}

Transform::~Transform()
{
	SetParent(nullptr); // 親から切り離す

	for (auto child : _children)
	{
		child->parent = nullptr; // 子供の親も無効化
	}
}


void Transform::SetParent(Transform* newParent)
{
	if (parent == newParent) return;

	GetWorldMatrix(); // 現在のワールド情報を確定させる

	if (parent)
	{
		auto& siblings = parent->_children;
		siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
	}

	parent = newParent;

	if (parent)
		parent->_children.push_back(this);

	isUpdating = true;
	UpdateLocalTransform(); // 新しい親に合わせてローカル情報を再計算
	isUpdating = false;

	MarkDirty(); // 再計算をトリガー
}

Transform* Transform::GetParent() const
{
	return parent;
}

XMFLOAT3 Transform::GetLocalPosition() const
{
	return _localPosition;
}

void Transform::SetLocalPosition(const XMFLOAT2& pos)
{
	SetLocalPosition(XMFLOAT3(pos.x, pos.y, _localPosition.z));
}

void Transform::SetLocalPosition(const XMFLOAT3& pos)
{
	if (isUpdating) return;

	_localPosition = pos;
	MarkDirty();

	isUpdating = true;
	UpdateWorldTransform(); // ローカル変更 → ワールドに反映
	isUpdating = false;
}

XMFLOAT3 Transform::GetPosition()
{
	if (isDirty)
		UpdateWorldMatrix(); // 最新ワールド位置を取得

	return worldPosition;
}

void Transform::SetPosition(const XMFLOAT2& pos)
{
	SetPosition(XMFLOAT3(pos.x, pos.y, worldPosition.z));
}

void Transform::SetPosition(const XMFLOAT3& pos)
{
	if (isUpdating) return;

	worldPosition = pos;

	isUpdating = true;
	UpdateLocalTransform(); // ワールド変更 → ローカル再計算
	isUpdating = false;

	MarkDirty();
}


float Transform::GetLocalRotation() const
{
	return _localRotation;
}

void Transform::SetLocalRotation(float rot)
{
	if (isUpdating) return;

	_localRotation = rot;
	MarkDirty();

	isUpdating = true;
	UpdateWorldTransform();
	isUpdating = false;
}

float Transform::GetRotation()
{
	if (isDirty)
		UpdateWorldMatrix();

	return worldRotation;
}

void Transform::SetRotation(float rot)
{
	if (isUpdating) return;

	worldRotation = rot;

	isUpdating = true;
	UpdateLocalTransform();
	isUpdating = false;

	MarkDirty();
}

XMFLOAT3 Transform::GetLocalScale()
{
	return _localScale;
}

void Transform::SetLocalScale(const DirectX::XMFLOAT2& scale)
{
	SetLocalScale(XMFLOAT3(scale.x, scale.y, _localScale.z));
}

void Transform::SetLocalScale(const XMFLOAT3& scale)
{
	if (isUpdating) return;

	_localScale = scale;
	MarkDirty();

	isUpdating = true;
	UpdateWorldTransform();
	isUpdating = false;
}

XMFLOAT3 Transform::GetScale()
{
	if (isDirty)
		UpdateWorldMatrix();

	return worldScale;
}

void Transform::SetScale(const XMFLOAT2& scale)
{
	SetScale(XMFLOAT3(scale.x, scale.y, worldScale.z));
}

void Transform::SetScale(const XMFLOAT3& scale)
{
	if (isUpdating) return;

	worldScale = scale;

	isUpdating = true;
	UpdateLocalTransform();
	isUpdating = false;

	MarkDirty();
}

const XMMATRIX& Transform::GetWorldMatrix()
{
	if (isDirty)
		UpdateWorldMatrix(); // 再計算が必要な場合のみ行う

	return worldMatrix;
}

const std::vector<Transform*>& Transform::GetChildren() const
{
	return _children;
}

void Transform::MarkDirty()
{
	if (!isDirty)
	{
		isDirty = true;

		// 子ノードすべてに「Dirty」状態を伝播
		for (auto child : _children)
		{
			child->MarkDirty();
		}
	}
}

void Transform::UpdateWorldMatrix()
{
	// ローカル行列の構築（スケール→回転→平行移動）
	XMMATRIX translationMatrix = XMMatrixTranslation(_localPosition.x, _localPosition.y, _localPosition.z);
	XMMATRIX rotationMatrix = XMMatrixRotationZ(XMConvertToRadians(_localRotation));
	XMMATRIX scaleMatrix = XMMatrixScaling(_localScale.x, _localScale.y, _localScale.z);
	XMMATRIX localMatrix = scaleMatrix * rotationMatrix * translationMatrix;

	// 親がいる場合、親のワールド行列と合成
	if (parent)
		worldMatrix = localMatrix * parent->GetWorldMatrix();
	else
		worldMatrix = localMatrix;

	if (parent)
	{
		// 回転、スケール、位置を親から継承して再計算
		worldRotation = parent->worldRotation + _localRotation;

		worldScale.x = parent->worldScale.x * _localScale.x;
		worldScale.y = parent->worldScale.y * _localScale.y;
		worldScale.z = parent->worldScale.z * _localScale.z;

		// 回転を考慮した相対位置計算
		float rad = XMConvertToRadians(parent->worldRotation);
		float cosA = cosf(rad);
		float sinA = sinf(rad);

		float rx = _localPosition.x * cosA - _localPosition.y * sinA;
		float ry = _localPosition.x * sinA + _localPosition.y * cosA;

		rx *= parent->worldScale.x;
		ry *= parent->worldScale.y;

		worldPosition.x = parent->worldPosition.x + rx;
		worldPosition.y = parent->worldPosition.y + ry;
		worldPosition.z = parent->worldPosition.z + _localPosition.z;
	}
	else
	{
		worldRotation = _localRotation;
		worldScale = _localScale;
		worldPosition = _localPosition;
	}

	isDirty = false; // 再計算完了
}

void Transform::UpdateWorldTransform()
{
	UpdateWorldMatrix();
}

void Transform::UpdateLocalTransform()
{
	if (parent)
	{
		// ワールド→ローカルへ変換（親の逆行列を使用）
		XMMATRIX parentWorldMatrix = parent->GetWorldMatrix();
		XMMATRIX parentInverseMatrix = XMMatrixInverse(nullptr, parentWorldMatrix);

		XMVECTOR worldPosVec = XMLoadFloat3(&worldPosition);
		XMVECTOR localPosVec = XMVector3TransformCoord(worldPosVec, parentInverseMatrix);
		XMStoreFloat3(&_localPosition, localPosVec);

		// スケール除算：子 / 親 = ローカル
		XMVECTOR worldScaleVec = XMLoadFloat3(&worldScale);
		XMVECTOR parentWorldScaleVec = XMLoadFloat3(&parent->worldScale);
		XMVECTOR localScaleVec = XMVectorDivide(worldScaleVec, parentWorldScaleVec);
		XMStoreFloat3(&_localScale, localScaleVec);

		_localRotation = worldRotation - parent->worldRotation;
	}
	else
	{
		// 親がなければローカル=ワールド
		_localPosition = worldPosition;
		_localScale = worldScale;
		_localRotation = worldRotation;
	}
}