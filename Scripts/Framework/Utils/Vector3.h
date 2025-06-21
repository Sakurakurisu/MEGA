#include <DirectXMath.h>
using namespace DirectX;

struct Vector3
{
	float x, y, z;

	Vector3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f)
		: x(_x), y(_y), z(_z) {}

	inline XMFLOAT3 ToXMFLOAT3() const {
		return XMFLOAT3(x, y, z);
	}

	inline XMVECTOR ToXMVECTOR() const {
		return XMVectorSet(x, y, z, 0.0f);
	}
};

