#pragma once

#include <DirectXMath.h>
#include <algorithm>

using namespace DirectX;

inline float Dot(const XMFLOAT3& vec1, const XMFLOAT3& vec2)
{
	float result{};
	XMVECTOR v1 = XMLoadFloat3(&vec1);
	XMVECTOR v2 = XMLoadFloat3(&vec2);
	XMStoreFloat(&result, XMVector3Dot(v1, v2));
	return result;
}

inline float Dot(const XMFLOAT2& vec1, const XMFLOAT2& vec2)
{
	float result{};
	XMVECTOR v1 = XMLoadFloat2(&vec1);
	XMVECTOR v2 = XMLoadFloat2(&vec2);
	XMStoreFloat(&result, XMVector3Dot(v1, v2));
	return result;
}

inline float Magnitude(const XMFLOAT2& vf2)
{
	float len{};
	XMVECTOR vec = XMLoadFloat2(&vf2);
	XMStoreFloat(&len, XMVector2Length(vec));
	return len;
}

inline float Magnitude(const XMFLOAT3& vf3)
{
	float len{};
	XMVECTOR vec = XMLoadFloat3(&vf3);
	XMStoreFloat(&len, XMVector3Length(vec));
	return len;
}

inline XMFLOAT3 Normalize(const XMFLOAT3& vf3)
{
	XMFLOAT3 result{ vf3 };
	XMVECTOR vec = XMLoadFloat3(&result);
	XMStoreFloat3(&result, XMVector3Normalize(vec));
	return result;
}

inline XMFLOAT2 Normalize(const XMFLOAT2& vf2)
{
	XMFLOAT2 result{ vf2 };
	XMVECTOR vec = XMLoadFloat2(&result);
	XMStoreFloat2(&result, XMVector2Normalize(vec));
	return result;
}

inline XMFLOAT3 Cross(const XMFLOAT3& vec1, const XMFLOAT3& vec2)
{
	XMFLOAT3 result{};
	XMVECTOR v1 = XMLoadFloat3(&vec1);
	XMVECTOR v2 = XMLoadFloat3(&vec2);
	XMStoreFloat3(&result, XMVector3Cross(v1, v2));
	return result;
}

#pragma region 2D
inline XMFLOAT2 operator-(const XMFLOAT2& v) noexcept
{
	return { -v.x, -v.y };
}

inline XMFLOAT2 operator-(const XMFLOAT2& v0, const XMFLOAT2& v1) noexcept
{
	return { v0.x - v1.x, v0.y - v1.y };
}

inline XMFLOAT2& operator-=(XMFLOAT2& v0, const XMFLOAT2& v1) noexcept
{
	v0.x -= v1.x;
	v0.y -= v1.y;

	return v0;
}

inline XMFLOAT2 operator+(const XMFLOAT2& v0, const XMFLOAT2& v1) noexcept
{
	return { v0.x + v1.x, v0.y + v1.y };
}

inline XMFLOAT2& operator+=(XMFLOAT2& v0, const XMFLOAT2& v1) noexcept
{
	v0.x += v1.x;
	v0.y += v1.y;

	return v0;
}

inline XMFLOAT2 operator*(const XMFLOAT2& v, float s) noexcept
{
	return { v.x * s, v.y * s };
}

inline XMFLOAT2 operator*(float s, const XMFLOAT2& v) noexcept
{
	return { s * v.x, s * v.y };
}

inline XMFLOAT2 operator*(const XMFLOAT2& v0, const XMFLOAT2& v1) noexcept
{
	return { v0.x * v1.x, v0.y * v1.y };
}

inline XMFLOAT2& operator*=(XMFLOAT2& v, float s) noexcept
{
	v.x *= s;
	v.y *= s;
	return v;
}

inline XMFLOAT2 operator/(const XMFLOAT2& v, float s) noexcept
{
	return v * (1.0f / s);
}

inline XMFLOAT2 operator/(const XMFLOAT2& v0, const XMFLOAT2& v1) noexcept
{
	return { v0.x / v1.x, v0.y / v1.y };
}
#pragma endregion

#pragma region 3D
inline XMFLOAT3 operator-(const XMFLOAT3& v) noexcept
{
	return { -v.x, -v.y, -v.z };
}

inline XMFLOAT3 operator-(const XMFLOAT3& v0, const XMFLOAT3& v1) noexcept
{
	return { v0.x - v1.x, v0.y - v1.y, v0.z - v1.z };
}

inline XMFLOAT3& operator-=(XMFLOAT3& v0, const XMFLOAT3& v1) noexcept
{
	v0.x -= v1.x;
	v0.y -= v1.y;
	v0.z -= v1.z;

	return v0;
}

inline XMFLOAT3 operator+(const XMFLOAT3& v0, const XMFLOAT3& v1) noexcept
{
	return { v0.x + v1.x, v0.y + v1.y, v0.z + v1.z };
}

inline XMFLOAT3& operator+=(XMFLOAT3& v0, const XMFLOAT3& v1) noexcept
{
	v0.x += v1.x;
	v0.y += v1.y;
	v0.z += v1.z;

	return v0;
}

inline XMFLOAT3 operator*(const XMFLOAT3& v, float s) noexcept
{
	return { v.x * s, v.y * s, v.z * s };
}

inline XMFLOAT3 operator*(float s, const XMFLOAT3& v) noexcept
{
	return { s * v.x, s * v.y, s * v.z };
}

inline XMFLOAT3 operator*(const XMFLOAT3& v0, const XMFLOAT3& v1) noexcept
{
	return { v0.x * v1.x, v0.y * v1.y, v0.z * v1.z };
}

inline XMFLOAT3& operator*=(XMFLOAT3& v, float s) noexcept
{
	v.x *= s;
	v.y *= s;
	v.z *= s;
	return v;
}

inline XMFLOAT3 operator/(const XMFLOAT3& v, float s) noexcept
{
	return v * (1.0f / s);
}

inline XMFLOAT3 operator/(const XMFLOAT3& v0, const XMFLOAT3& v1) noexcept
{
	return { v0.x / v1.x, v0.y / v1.y, v0.z / v1.z };
}

#pragma endregion

#pragma region 2D && 3D
inline XMFLOAT3 operator+(const XMFLOAT2& vec2, const XMFLOAT3& vec3)
{
	XMFLOAT3 extendedVec2(vec2.x, vec2.y, 0.0f);

	XMVECTOR v2 = XMLoadFloat3(&extendedVec2);
	XMVECTOR v3 = XMLoadFloat3(&vec3);
	XMVECTOR result = XMVectorAdd(v2, v3);

	XMFLOAT3 resultVec{};
	XMStoreFloat3(&resultVec, result);
	return resultVec;
}

inline XMFLOAT3 operator+(const XMFLOAT3& vec3, const XMFLOAT2& vec2)
{
	return vec2 + vec3;
}

inline XMFLOAT3& operator+=(XMFLOAT3& v0, const XMFLOAT2& v1) noexcept
{
	v0.x += v1.x;
	v0.y += v1.y;

	return v0;
}

inline XMFLOAT3 operator-(const XMFLOAT2& vec2, const XMFLOAT3& vec3)
{
	XMFLOAT3 extendedVec2(vec2.x, vec2.y, 0.0f);

	XMVECTOR v2 = XMLoadFloat3(&extendedVec2);
	XMVECTOR v3 = XMLoadFloat3(&vec3);
	XMVECTOR result = XMVectorSubtract(v2, v3);

	XMFLOAT3 resultVec{};
	XMStoreFloat3(&resultVec, result);
	return resultVec;
}

inline XMFLOAT3 operator-(const XMFLOAT3& vec3, const XMFLOAT2& vec2)
{
	XMFLOAT3 extendedVec2(vec2.x, vec2.y, 0.0f);

	XMVECTOR v3 = XMLoadFloat3(&vec3);
	XMVECTOR v2 = XMLoadFloat3(&extendedVec2);
	XMVECTOR result = XMVectorSubtract(v3, v2);

	XMFLOAT3 resultVec{};
	XMStoreFloat3(&resultVec, result);
	return resultVec;
}

inline XMFLOAT3& operator-=(XMFLOAT3& v0, const XMFLOAT2& v1) noexcept
{
	v0.x -= v1.x;
	v0.y -= v1.y;

	return v0;
}
#pragma endregion

#pragma region 4D

inline XMFLOAT4 operator-(const XMFLOAT4& v) noexcept
{
	return { -v.x, -v.y, -v.z, -v.w };
}

inline XMFLOAT4 operator+(const XMFLOAT4& v0, const XMFLOAT4& v1) noexcept
{
	return { v0.x + v1.x, v0.y + v1.y, v0.z + v1.z, v0.w + v1.w };
}

inline XMFLOAT4& operator+=(XMFLOAT4& v0, const XMFLOAT4& v1) noexcept
{
	v0.x += v1.x;
	v0.y += v1.y;
	v0.z += v1.z;
	v0.w += v1.w;
	return v0;
}

inline XMFLOAT4 operator-(const XMFLOAT4& v0, const XMFLOAT4& v1) noexcept
{
	return { v0.x - v1.x, v0.y - v1.y, v0.z - v1.z, v0.w - v1.w };
}

inline XMFLOAT4& operator-=(XMFLOAT4& v0, const XMFLOAT4& v1) noexcept
{
	v0.x -= v1.x;
	v0.y -= v1.y;
	v0.z -= v1.z;
	v0.w -= v1.w;
	return v0;
}

#pragma endregion

#pragma region 4D Multiplication

inline XMFLOAT4 operator*(const XMFLOAT4& v, float s) noexcept
{
	return { v.x * s, v.y * s, v.z * s, v.w * s };
}

inline XMFLOAT4 operator*(float s, const XMFLOAT4& v) noexcept
{
	return { s * v.x, s * v.y, s * v.z, s * v.w };
}

inline XMFLOAT4 operator*(const XMFLOAT4& v0, const XMFLOAT4& v1) noexcept
{
	return { v0.x * v1.x, v0.y * v1.y, v0.z * v1.z, v0.w * v1.w };
}

#pragma endregion

inline XMFLOAT3 Bezier3(float t, const XMFLOAT3& p1, const XMFLOAT3& p2, const XMFLOAT3& p3, const XMFLOAT3& p4)
{
	float d = 1.0f - t;
	return d * d * d * p1 + 3 * d * d * t * p2 + 3 * d * t * t * p3 + t * t * t * p4;
}

inline XMFLOAT3 Lerp(const XMFLOAT3& start, const XMFLOAT3& end, float t)
{
	return (1.0f - t) * start + t * end;
}

inline float LerpAngle(float current, float target, float t)
{
	current = fmodf(current, 360.0f);
	if (current < 0) current += 360.0f;

	target = fmodf(target, 360.0f);
	if (target < 0) target += 360.0f;

	float diff = target - current;
	if (diff > 180.0f)
		diff -= 360.0f;
	else if (diff < -180.0f)
		diff += 360.0f;

	return current + diff * t;
}

inline XMFLOAT3 Slerp(float t, const XMFLOAT3& start, const XMFLOAT3& end)
{
	float angle = acosf(Dot(Normalize(start), Normalize(end)));
	float sin_th = sinf(angle);

	if (fabsf(sin_th) < 0.001f) sin_th = 0.001f;

	float ps = sinf(angle * (1.0f - t));
	float pe = sinf(angle * t);

	return (ps * start + pe * end) / sin_th;
}

inline XMVECTOR ToXMVECTOR(const XMFLOAT3& vec)
{
	return XMLoadFloat3(&vec);
}

inline XMFLOAT3 ToXMFLOAT3(const XMVECTOR& vec)
{
	XMFLOAT3 result{};
	XMStoreFloat3(&result, vec);
	return result;
}

inline XMFLOAT3 ToXMFLOAT3(const XMFLOAT2& vec2, float z = 0.0f)
{
	return { vec2.x, vec2.y, z };
}

inline XMFLOAT2 ToXMFLOAT2(const XMFLOAT3& vec3)
{
	return { vec3.x, vec3.y };
}

inline XMFLOAT2 SetMag(XMFLOAT2& vec, float len)
{
	vec = Normalize(vec) * len;
	return vec;
}

inline XMFLOAT2 LimitVec(XMFLOAT2& vec, float lim)
{
	if (Magnitude(vec) > lim)
	{
		vec = Normalize(vec) * lim;
	}
	return vec;
}

inline float Distance(const XMFLOAT3& vec1, const XMFLOAT3& vec2)
{
	float result{};
	XMVECTOR v1 = XMLoadFloat3(&vec1);
	XMVECTOR v2 = XMLoadFloat3(&vec2);
	XMVECTOR diff = XMVectorSubtract(v1, v2);
	XMVECTOR len = XMVector3Length(diff);
	XMStoreFloat(&result, len);
	return result;
}

inline float Distance(const XMFLOAT2& vec1, const XMFLOAT2& vec2)
{
	float result{};
	XMVECTOR v1 = XMLoadFloat2(&vec1);
	XMVECTOR v2 = XMLoadFloat2(&vec2);
	XMVECTOR diff = XMVectorSubtract(v1, v2);
	XMVECTOR len = XMVector2Length(diff);
	XMStoreFloat(&result, len);
	return result;
}

inline XMFLOAT3 Direction(const XMFLOAT3& from, const XMFLOAT3& to)
{
	XMFLOAT3 dir = to - from;
	return Normalize(dir);
}

inline XMFLOAT2 Direction(const XMFLOAT2& from, const XMFLOAT2& to)
{
	XMFLOAT2 dir = to - from;
	return Normalize(dir);
}

inline float Angle(const XMFLOAT2& vec1, const XMFLOAT2& vec2)
{
	XMFLOAT2 normalizedVec1 = Normalize(vec1);
	XMFLOAT2 normalizedVec2 = Normalize(vec2);
	float dotProduct = Dot(normalizedVec1, normalizedVec2);
	dotProduct = std::clamp(dotProduct, -1.0f, 1.0f);
	return XMConvertToDegrees(acosf(dotProduct));
}

inline float Angle(const XMFLOAT3& vec1, const XMFLOAT3& vec2)
{
	XMFLOAT3 normalizedVec1 = Normalize(vec1);
	XMFLOAT3 normalizedVec2 = Normalize(vec2);
	float dotProduct = Dot(normalizedVec1, normalizedVec2);
	dotProduct = std::clamp(dotProduct, -1.0f, 1.0f);
	return XMConvertToDegrees(acosf(dotProduct));
}

inline float SignedAngle(const XMFLOAT3& vec1, const XMFLOAT3& vec2, const XMFLOAT3& axis)
{
	XMFLOAT3 normalizedVec1 = Normalize(vec1);
	XMFLOAT3 normalizedVec2 = Normalize(vec2);
	float dot = Dot(normalizedVec1, normalizedVec2);
	XMFLOAT3 cross = Cross(normalizedVec1, normalizedVec2);
	float angle = XMConvertToDegrees(atan2f(Magnitude(cross), dot));
	float sign = Dot(cross, axis);
	return sign >= 0.0f ? angle : -angle;
}

inline float SignedAngle(const XMFLOAT2& vec1, const XMFLOAT2& vec2)
{
	XMFLOAT2 normalizedVec1 = Normalize(vec1);
	XMFLOAT2 normalizedVec2 = Normalize(vec2);
	float dot = Dot(normalizedVec1, normalizedVec2);
	float cross = normalizedVec1.x * normalizedVec2.y - normalizedVec1.y * normalizedVec2.x;
	return XMConvertToDegrees(atan2f(cross, dot));
    #pragma once  

    #include <DirectXMath.h>  
    #include <algorithm>  

    using namespace DirectX;  

    // ベクトルの内積を計算する関数 (3D)
    inline float Dot(const XMFLOAT3& vec1, const XMFLOAT3& vec2)  
    {  
       float result{};  
       XMVECTOR v1 = XMLoadFloat3(&vec1);  
       XMVECTOR v2 = XMLoadFloat3(&vec2);  
       XMStoreFloat(&result, XMVector3Dot(v1, v2));  
       return result;  
    }  

    // ベクトルの内積を計算する関数 (2D)
    inline float Dot(const XMFLOAT2& vec1, const XMFLOAT2& vec2)  
    {  
       float result{};  
       XMVECTOR v1 = XMLoadFloat2(&vec1);  
       XMVECTOR v2 = XMLoadFloat2(&vec2);  
       XMStoreFloat(&result, XMVector3Dot(v1, v2));  
       return result;  
    }  

    // ベクトルの大きさを計算する関数 (2D)
    inline float Magnitude(const XMFLOAT2& vf2)  
    {  
       float len{};  
       XMVECTOR vec = XMLoadFloat2(&vf2);  
       XMStoreFloat(&len, XMVector2Length(vec));  
       return len;  
    }  

    // ベクトルの大きさを計算する関数 (3D)
    inline float Magnitude(const XMFLOAT3& vf3)  
    {  
       float len{};  
       XMVECTOR vec = XMLoadFloat3(&vf3);  
       XMStoreFloat(&len, XMVector3Length(vec));  
       return len;  
    }  

    // ベクトルを正規化する関数 (3D)
    inline XMFLOAT3 Normalize(const XMFLOAT3& vf3)  
    {  
       XMFLOAT3 result{ vf3 };  
       XMVECTOR vec = XMLoadFloat3(&result);  
       XMStoreFloat3(&result, XMVector3Normalize(vec));  
       return result;  
    }  

    // ベクトルを正規化する関数 (2D)
    inline XMFLOAT2 Normalize(const XMFLOAT2& vf2)  
    {  
       XMFLOAT2 result{ vf2 };  
       XMVECTOR vec = XMLoadFloat2(&result);  
       XMStoreFloat2(&result, XMVector2Normalize(vec));  
       return result;  
    }  

    // ベクトルの外積を計算する関数 (3D)
    inline XMFLOAT3 Cross(const XMFLOAT3& vec1, const XMFLOAT3& vec2)  
    {  
       XMFLOAT3 result{};  
       XMVECTOR v1 = XMLoadFloat3(&vec1);  
       XMVECTOR v2 = XMLoadFloat3(&vec2);  
       XMStoreFloat3(&result, XMVector3Cross(v1, v2));  
       return result;  
    }  

    #pragma region 2D  
    // 2Dベクトルの符号反転
    inline XMFLOAT2 operator-(const XMFLOAT2& v) noexcept  
    {  
       return { -v.x, -v.y };  
    }  

    // 2Dベクトルの減算
    inline XMFLOAT2 operator-(const XMFLOAT2& v0, const XMFLOAT2& v1) noexcept  
    {  
       return { v0.x - v1.x, v0.y - v1.y };  
    }  

    // 2Dベクトルの減算代入
    inline XMFLOAT2& operator-=(XMFLOAT2& v0, const XMFLOAT2& v1) noexcept  
    {  
       v0.x -= v1.x;  
       v0.y -= v1.y;  

       return v0;  
    }  

    // 2Dベクトルの加算
    inline XMFLOAT2 operator+(const XMFLOAT2& v0, const XMFLOAT2& v1) noexcept  
    {  
       return { v0.x + v1.x, v0.y + v1.y };  
    }  

    // 2Dベクトルの加算代入
    inline XMFLOAT2& operator+=(XMFLOAT2& v0, const XMFLOAT2& v1) noexcept  
    {  
       v0.x += v1.x;  
       v0.y += v1.y;  

       return v0;  
    }  

    // 2Dベクトルのスカラー倍
    inline XMFLOAT2 operator*(const XMFLOAT2& v, float s) noexcept  
    {  
       return { v.x * s, v.y * s };  
    }  

    // スカラーと2Dベクトルの積
    inline XMFLOAT2 operator*(float s, const XMFLOAT2& v) noexcept  
    {  
       return { s * v.x, s * v.y };  
    }  

    // 2Dベクトル同士の積
    inline XMFLOAT2 operator*(const XMFLOAT2& v0, const XMFLOAT2& v1) noexcept  
    {  
       return { v0.x * v1.x, v0.y * v1.y };  
    }  

    // 2Dベクトルのスカラー倍代入
    inline XMFLOAT2& operator*=(XMFLOAT2& v, float s) noexcept  
    {  
       v.x *= s;  
       v.y *= s;  
       return v;  
    }  

    // 2Dベクトルのスカラー除算
    inline XMFLOAT2 operator/(const XMFLOAT2& v, float s) noexcept  
    {  
       return v * (1.0f / s);  
    }  

    // 2Dベクトル同士の除算
    inline XMFLOAT2 operator/(const XMFLOAT2& v0, const XMFLOAT2& v1) noexcept  
    {  
       return { v0.x / v1.x, v0.y / v1.y };  
    }  
    #pragma endregion  

    // 他の関数や演算子についても同様にコメントを追加できます。
}