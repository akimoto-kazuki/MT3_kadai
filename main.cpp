#include <Novice.h>
#include <cstdint>
#define _USE_MATH_DEFINES
#include <cmath>
#include <assert.h>
#include <imgui.h>
#include <algorithm>


const char kWindowTitle[] = "LC1C_02_アキモト_カズキ";

struct Vector3
{
	float x;
	float y;
	float z;
};

struct Matrix4x4
{
	float m[4][4];
};

struct Sphere
{
	Vector3 center;
	float radius;
};

struct Segment
{
	Vector3 origin;
	Vector3 diff;
};

struct Plane
{
	Vector3 normal; // 法線
	float distance; // 距離
};

struct Triangle
{
	Vector3 vertices[3]; // 頂点
};

struct AABB
{
	Vector3 min;
	Vector3 max;
};

float Dot(const Vector3& v1, const Vector3& v2)
{
	float result;
	result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	return result;
}

Vector3 rotate_{ 0.0f,0.0f,0.0f };

Vector3 translate_{ 0.0f,0.0f,0.0f };

Vector3 kLocalVertices[3]{ {0.0f,1.0f,0.0f},{-1.0f,0.0f,0.0f},{1.0f,0.0f,0.0f} };
// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

Matrix4x4 Add4x4(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result;
	result.m[0][0] = m1.m[0][0] + m2.m[0][0]; result.m[0][1] = m1.m[0][1] + m2.m[0][1]; result.m[0][2] = m1.m[0][2] + m2.m[0][2]; result.m[0][3] = m1.m[0][3] + m2.m[0][3];
	result.m[1][0] = m1.m[1][0] + m2.m[1][0]; result.m[1][1] = m1.m[1][1] + m2.m[1][1]; result.m[1][2] = m1.m[1][2] + m2.m[1][2]; result.m[1][3] = m1.m[1][3] + m2.m[1][3];
	result.m[2][0] = m1.m[2][0] + m2.m[2][0]; result.m[2][1] = m1.m[2][1] + m2.m[2][1]; result.m[2][2] = m1.m[2][2] + m2.m[2][2]; result.m[2][3] = m1.m[2][3] + m2.m[2][3];
	result.m[3][0] = m1.m[3][0] + m2.m[3][0]; result.m[3][1] = m1.m[3][1] + m2.m[3][1]; result.m[3][2] = m1.m[3][2] + m2.m[3][2]; result.m[3][3] = m1.m[3][3] + m2.m[3][3];

	return result;
}

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

Matrix4x4 Subtract4x4(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result;
	result.m[0][0] = m1.m[0][0] - m2.m[0][0]; result.m[0][1] = m1.m[0][1] - m2.m[0][1]; result.m[0][2] = m1.m[0][2] - m2.m[0][2]; result.m[0][3] = m1.m[0][3] - m2.m[0][3];
	result.m[1][0] = m1.m[1][0] - m2.m[1][0]; result.m[1][1] = m1.m[1][1] - m2.m[1][1]; result.m[1][2] = m1.m[1][2] - m2.m[1][2]; result.m[1][3] = m1.m[1][3] - m2.m[1][3];
	result.m[2][0] = m1.m[2][0] - m2.m[2][0]; result.m[2][1] = m1.m[2][1] - m2.m[2][1]; result.m[2][2] = m1.m[2][2] - m2.m[2][2]; result.m[2][3] = m1.m[2][3] - m2.m[2][3];
	result.m[3][0] = m1.m[3][0] - m2.m[3][0]; result.m[3][1] = m1.m[3][1] - m2.m[3][1]; result.m[3][2] = m1.m[3][2] - m2.m[3][2]; result.m[3][3] = m1.m[3][3] - m2.m[3][3];

	return result;
}

float Length(const Vector3& v)
{
	float result;
	result = static_cast<float>(sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
	return result;
}

Vector3 Normalize(const Vector3& v)
{
	Vector3 result;
	result.x = v.x / static_cast<float>(sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
	result.y = v.y / static_cast<float>(sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
	result.z = v.z / static_cast<float>(sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
	return result;
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result;

	result.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0];
	result.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1];
	result.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2];
	result.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3];

	result.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0];
	result.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1];
	result.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
	result.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3];

	result.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0];
	result.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1];
	result.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
	result.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3];

	result.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0];
	result.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1];
	result.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];
	result.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3];

	return result;

}

Vector3 MultiplyVector3(float scalar, const Vector3& v)
{
	Vector3 result;
	result.x = scalar * v.x;
	result.y = scalar * v.y;
	result.z = scalar * v.z;
	return result;
}

Matrix4x4 Inverse(const Matrix4x4& m)
{
	Matrix4x4 result;
	float a;
	a =
		m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2]
		- m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]
		- m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]
		+ m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2]
		+ m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2]
		- m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]
		- m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]
		+ m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

	result.m[0][0] =
		(m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[1][3] * m.m[2][1] * m.m[3][2]
			- m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]) / a;
	result.m[0][1] =
		(-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[2][1] * m.m[3][2]
			+ m.m[0][3] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]) / a;
	result.m[0][2] =
		(m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[3][2]
			- m.m[0][3] * m.m[1][2] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]) / a;
	result.m[0][3] =
		(-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] - m.m[0][3] * m.m[1][1] * m.m[2][2]
			+ m.m[0][3] * m.m[1][2] * m.m[2][1] + m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]) / a;

	result.m[1][0] =
		(-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[1][3] * m.m[2][0] * m.m[3][2]
			+ m.m[1][3] * m.m[2][2] * m.m[3][0] + m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]) / a;
	result.m[1][1] =
		(m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] + m.m[0][3] * m.m[2][0] * m.m[3][2]
			- m.m[0][3] * m.m[2][2] * m.m[3][0] - m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]) / a;
	result.m[1][2] =
		(-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] - m.m[0][3] * m.m[1][0] * m.m[3][2]
			+ m.m[0][3] * m.m[1][2] * m.m[3][0] + m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]) / a;
	result.m[1][3] =
		(m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] + m.m[0][3] * m.m[1][0] * m.m[2][2]
			- m.m[0][3] * m.m[1][2] * m.m[2][0] - m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]) / a;

	result.m[2][0] =
		(m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[1][3] * m.m[2][0] * m.m[3][1]
			- m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]) / a;
	result.m[2][1] =
		(-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] - m.m[0][3] * m.m[2][0] * m.m[3][1]
			+ m.m[0][3] * m.m[2][1] * m.m[3][0] + m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]) / a;
	result.m[2][2] =
		(m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] + m.m[0][3] * m.m[1][0] * m.m[3][1]
			- m.m[0][3] * m.m[1][1] * m.m[3][0] - m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]) / a;
	result.m[2][3] =
		(-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] - m.m[0][3] * m.m[1][0] * m.m[2][1]
			+ m.m[0][3] * m.m[1][1] * m.m[2][0] + m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]) / a;

	result.m[3][0] =
		(-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] - m.m[1][2] * m.m[2][0] * m.m[3][1]
			+ m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]) / a;
	result.m[3][1] =
		(m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] + m.m[0][2] * m.m[2][0] * m.m[3][1]
			- m.m[0][2] * m.m[2][1] * m.m[3][0] - m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]) / a;
	result.m[3][2] =
		(-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] - m.m[0][2] * m.m[1][0] * m.m[3][1]
			+ m.m[0][2] * m.m[1][1] * m.m[3][0] + m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]) / a;
	result.m[3][3] =
		(m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] + m.m[0][2] * m.m[1][0] * m.m[2][1]
			- m.m[0][2] * m.m[1][1] * m.m[2][0] - m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]) / a;
	return result;
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix)
{
	Vector3 result;

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float  w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate)
{
	Matrix4x4 result;

	result.m[0][0] = 1.0f; result.m[0][1] = 0.0f; result.m[0][2] = 0.0f; result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f; result.m[1][1] = 1.0f; result.m[1][2] = 0.0f; result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f; result.m[2][1] = 0.0f; result.m[2][2] = 1.0f; result.m[2][3] = 0.0f;

	result.m[3][0] = translate.x; result.m[3][1] = translate.y; result.m[3][2] = translate.z; result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 MakeRotateXMatrix(float radian)
{
	Matrix4x4 result;

	result.m[0][0] = 1.0f; result.m[0][1] = 0.0f; result.m[0][2] = 0.0f; result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f; result.m[1][1] = std::cos(radian); result.m[1][2] = std::sin(radian); result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f; result.m[2][1] = -std::sin(radian); result.m[2][2] = std::cos(radian); result.m[2][3] = 0.0f;
	result.m[3][0] = 0.0f; result.m[3][1] = 0.0f; result.m[3][2] = 0.0f; result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 MakeRotateYMatrix(float radian)
{
	Matrix4x4 result;

	result.m[0][0] = std::cos(radian); result.m[0][1] = 0.0f; result.m[0][2] = -std::sin(radian); result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f; result.m[1][1] = 1.0f; result.m[1][2] = 0.0f; result.m[1][3] = 0.0f;
	result.m[2][0] = std::sin(radian); result.m[2][1] = 0.0f; result.m[2][2] = std::cos(radian); result.m[2][3] = 0.0f;
	result.m[3][0] = 0.0f; result.m[3][1] = 0.0f; result.m[3][2] = 0.0f; result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 MakeRotateZMatrix(float radian)
{
	Matrix4x4 result;

	result.m[0][0] = std::cos(radian); result.m[0][1] = std::sin(radian); result.m[0][2] = 0.0f; result.m[0][3] = 0.0f;
	result.m[1][0] = -std::sin(radian); result.m[1][1] = std::cos(radian); result.m[1][2] = 0.0f; result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f; result.m[2][1] = 0.0f; result.m[2][2] = 1.0f; result.m[2][3] = 0.0f;
	result.m[3][0] = 0.0f; result.m[3][1] = 0.0f; result.m[3][2] = 0.0f; result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{
	Matrix4x4 result;

	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 XYZ = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

	result.m[0][0] = scale.x * XYZ.m[0][0]; result.m[0][1] = scale.x * XYZ.m[0][1]; result.m[0][2] = scale.x * XYZ.m[0][2]; result.m[0][3] = 0.0f;
	result.m[1][0] = scale.y * XYZ.m[1][0]; result.m[1][1] = scale.y * XYZ.m[1][1]; result.m[1][2] = scale.y * XYZ.m[1][2]; result.m[1][3] = 0.0f;
	result.m[2][0] = scale.z * XYZ.m[2][0]; result.m[2][1] = scale.z * XYZ.m[2][1]; result.m[2][2] = scale.z * XYZ.m[2][2]; result.m[2][3] = 0.0f;
	result.m[3][0] = translate.x; result.m[3][1] = translate.y; result.m[3][2] = translate.z; result.m[3][3] = 1.0f;

	return result;
}
//透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip)
{
	Matrix4x4 result;


	result.m[0][0] = 1/aspectRatio * 1/tan(fovY / 2); result.m[0][1] = 0.0f; result.m[0][2] = 0.0f; result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f; result.m[1][1] = 1/tan(fovY / 2); result.m[1][2] = 0.0f; result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f; result.m[2][1] = 0.0f; result.m[2][2] = farClip /(farClip - nearClip); result.m[2][3] = 1.0f;
	result.m[3][0] = 0.0f; result.m[3][1] = 0.0f; result.m[3][2] = -nearClip * farClip / (farClip - nearClip); result.m[3][3] = 0.0f;
  
	return result;
}
//正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip)
{
	Matrix4x4 result;


	result.m[0][0] = 2/(right - left); result.m[0][1] = 0.0f; result.m[0][2] = 0.0f; result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f; result.m[1][1] = 2/(top-bottom); result.m[1][2] = 0.0f; result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f; result.m[2][1] = 0.0f; result.m[2][2] = 1/(farClip - nearClip); result.m[2][3] = 0.0f;
	result.m[3][0] = (left + right)/(left - right); result.m[3][1] = (top + bottom)/(bottom - top); result.m[3][2] = nearClip/(nearClip-farClip); result.m[3][3] = 1.0f;
	return result;
}
//ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
{
	Matrix4x4 result;

	result.m[0][0] = width / 2; result.m[0][1] = 0.0f; result.m[0][2] = 0.0f; result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f; result.m[1][1] = -height/2; result.m[1][2] = 0.0f; result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f; result.m[2][1] = 0.0f; result.m[2][2] = maxDepth - minDepth; result.m[2][3] = 0.0f;
	result.m[3][0] = left + width/2; result.m[3][1] = top + height/2; result.m[3][2] = minDepth; result.m[3][3] = 1.0f;

	return result;
}

Vector3 Cross(const Vector3& v1, const Vector3& v2)
{
	Vector3 reslut;

	reslut.x = v1.y * v2.z - v1.z * v2.y;
	reslut.y = v1.z * v2.x - v1.x * v2.z;
	reslut.z = v1.x * v2.y - v1.y * v2.x;

	return reslut;
}

Vector3 Project(const Vector3& v1, const Vector3& v2)
{
	
	Vector3 pro;
	pro.x = (Dot(v1, v2) / Dot(v2, v2)) * v2.x;
	pro.y = (Dot(v1, v2) / Dot(v2, v2)) * v2.y;
	pro.z = (Dot(v1, v2) / Dot(v2, v2)) * v2.z;

	return pro;

}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment)
{
	Vector3 a;
	float t;
	Vector3 tb;
	Vector3 cp;

	a = Subtract(point, segment.origin);
	t = Dot(a, segment.diff) / Dot(segment.diff, segment.diff);
	tb.x = t * segment.diff.x;
	tb.y = t * segment.diff.y;
	tb.z = t * segment.diff.z;
	cp = Add(segment.origin,tb);

	return cp;

}

Vector3 Perpendicular(const Vector3& vector)
{
	if (vector.x != 0.0f || vector.y != 0.0f)
	{
		return { -vector.y , vector.x,0.0f };
	}
	return { 0.0f,-vector.z,vector.y };
}

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t)
{
	return Add(MultiplyVector3(t, v1), MultiplyVector3((1.0f - t), v2));
}

Vector3 Bezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, float t)
{
	Vector3 p0p1 = Lerp(p0, p1, t);
	Vector3 p1p2 = Lerp(p1, p2, t);
	Vector3 p = Lerp(p0p1, p1p2, t);

	return p;
}

// 演算子オーバーロード
Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	return Add(v1, v2);
}

Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	return Subtract(v1, v2);
}

Vector3 operator*(float s, const Vector3& v)
{
	return MultiplyVector3(s, v);
}

Vector3 operator*(const Vector3& v,float s)
{
	return s * v;
}

Vector3 operator/(const Vector3& v, float s)
{
	return MultiplyVector3(1.0f / s, v);
}

Matrix4x4 operator+ (const Matrix4x4& m1, const Matrix4x4& m2)
{
	return Add4x4(m1, m2);
}

Matrix4x4 operator- (const Matrix4x4& m1, const Matrix4x4& m2)
{
	return Subtract4x4(m1, m2);
}

Matrix4x4 operator* (const Matrix4x4& m1, const Matrix4x4& m2)
{
	return Multiply(m1, m2);
}

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label)
{
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix)
{
	const float kGridHalfWidth = 2.0f;										// Gridの半分の幅
	const uint32_t kSubdivision = 10;										// 分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision); // 1つ分の長さ

	Vector3 localStart;

	Vector3 localEnd;

	// 奥から手前への線を順々に引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex)
	{
		// 上の情報を使ってワールド座標系上の始点を終点を求める
		
		//Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0,1.0,1.0 }, rotate_, translate_);

		// スクリーン座標系まで変換をかける
		
		//Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);

		localStart.x = -kGridHalfWidth + ((xIndex)*kGridEvery);

		localStart.y = 0.0f;

		localStart.z = -kGridHalfWidth;

		localEnd.x = -kGridHalfWidth + ((xIndex)*kGridEvery);

		localEnd.y = 0.0f;

		localEnd.z = kGridHalfWidth;

		Vector3 screenStart = Transform(Transform(localStart, viewProjectionMatrix), viewportMatrix);

		Vector3 screenEnd = Transform(Transform(localEnd, viewProjectionMatrix), viewportMatrix);

		// 変換した座標を使って表示。色の薄い灰色(0xAAAAAAFF)、原点は黒で良いが、何でも良い
		Novice::DrawLine(int(screenStart.x), int(screenStart.y), int(screenEnd.x), int(screenEnd.y), 0xAAAAAAFF);

		if (xIndex == 5)
		{
			Novice::DrawLine(int(screenStart.x), int(screenStart.y), int(screenEnd.x), int(screenEnd.y), BLACK);
		}
	}
	// 左から右も同じように順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex)
	{
		// 奥から手前が左右に変わるだけ
		localStart.x = kGridHalfWidth;

		localStart.y = 0.0f;

		localStart.z = -kGridHalfWidth + ((zIndex)*kGridEvery);

		localEnd.x = -kGridHalfWidth;

		localEnd.y = 0.0f;

		localEnd.z = -kGridHalfWidth + ((zIndex)*kGridEvery);

		Vector3 screenStart = Transform(Transform(localStart, viewProjectionMatrix), viewportMatrix);

		Vector3 screenEnd = Transform(Transform(localEnd, viewProjectionMatrix), viewportMatrix);

		// 変換した座標を使って表示。色の薄い灰色(0xAAAAAAFF)、原点は黒で良いが、何でも良い
		Novice::DrawLine(int(screenStart.x), int(screenStart.y), int(screenEnd.x), int(screenEnd.y), 0xAAAAAAFF);

		if (zIndex == 5)
		{
			Novice::DrawLine(int(screenStart.x), int(screenStart.y), int(screenEnd.x), int(screenEnd.y), BLACK);
		}

	}

}

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	const uint32_t kSubdivision = 10;						// 分割数
	const float kLonEvery = float(2*M_PI) / kSubdivision;	// 経度分割１つの角度
	const float kLatEvery = float(M_PI) / kSubdivision;		// 緯度分割１つの角度
	const float pi = float(M_PI);
	// 緯度の方向に分割 -π/2 ~ π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex)
	{
		float lat = -pi / 2.0f + kLatEvery * latIndex;
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex)
		{
			float lon = lonIndex * kLonEvery;
			Vector3 a, b, c;
			a = { 
				sphere.center.x + sphere.radius * cosf(lat) * cosf(lon),
				sphere.center.y + sphere.radius * sinf(lat),
				sphere.center.z + sphere.radius * cosf(lat) * sinf(lon)
			};
			b = { 
				sphere.center.x + sphere.radius * cosf(lat + kLatEvery) * cosf(lon),
				sphere.center.y + sphere.radius * sinf(lat + kLatEvery),
				sphere.center.z + sphere.radius * cosf(lat + kLatEvery) * sinf(lon)
			};
			c = { 
				sphere.center.x + sphere.radius * cosf(lat) * cosf(lon + kLonEvery),
				sphere.center.y + sphere.radius * sinf(lat),
				sphere.center.z + sphere.radius * cosf(lat) * sinf(lon + kLonEvery)
			};
			Vector3 screenA = Transform(Transform(a, viewProjectionMatrix), viewportMatrix);
			Vector3 screenB = Transform(Transform(b, viewProjectionMatrix), viewportMatrix);
			Vector3 screenC = Transform(Transform(c, viewProjectionMatrix), viewportMatrix);

			Novice::DrawLine(int(screenA.x), int(screenA.y), int(screenB.x), int(screenB.y), color);
			Novice::DrawLine(int(screenA.x), int(screenA.y), int(screenC.x), int(screenC.y), color);

		}
	}
}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Vector3 center = MultiplyVector3(plane.distance, plane.normal);
	Vector3 perpendiculars[4];
	perpendiculars[0] = Normalize(Perpendicular(plane.normal));
	perpendiculars[1] = { -perpendiculars[0].x,-perpendiculars[0].y,-perpendiculars[0].z };
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);
	perpendiculars[3] = { -perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z };

	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index)
	{
		Vector3 extend = MultiplyVector3(2.0f, perpendiculars[index]);
		Vector3 point = Add(center, extend);
		points[index] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
	}
	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[3].x), int(points[3].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y), int(points[0].x), int(points[0].y), color);
	Novice::DrawLine(int(points[3].x), int(points[3].y), int(points[1].x), int(points[1].y), color);
}

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Vector3 screenA = Transform(Transform(triangle.vertices[0], viewProjectionMatrix), viewportMatrix);
	Vector3 screenB = Transform(Transform(triangle.vertices[1], viewProjectionMatrix), viewportMatrix);
	Vector3 screenC = Transform(Transform(triangle.vertices[2], viewProjectionMatrix), viewportMatrix);

	Novice::DrawTriangle(
		int(screenA.x), int(screenA.y), int(screenB.x), int(screenB.y),
		int(screenC.x), int(screenC.y), color, kFillModeWireFrame);

}

void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Vector3 aabb0; // minの位置
	aabb0 = { aabb.min.x,aabb.min.y,aabb.min.z };
	Vector3 screenaabb0;
	screenaabb0 = Transform(Transform(aabb0, viewProjectionMatrix), viewportMatrix);

	Vector3 aabb1; // minの上
	aabb1 = { aabb.min.x,aabb.max.y,aabb.min.z };
	Vector3 screenaabb1;
	screenaabb1 = Transform(Transform(aabb1, viewProjectionMatrix), viewportMatrix);

	Vector3 aabb2; // minの右
	aabb2 = { aabb.max.x,aabb.min.y,aabb.min.z };
	Vector3 screenaabb2;
	screenaabb2 = Transform(Transform(aabb2, viewProjectionMatrix), viewportMatrix);

	Vector3 aabb3; // minの右上
	aabb3 = { aabb.max.x,aabb.max.y,aabb.min.z };
	Vector3 screenaabb3;
	screenaabb3 = Transform(Transform(aabb3, viewProjectionMatrix), viewportMatrix);

	Vector3 aabb4; // maxの左下
	aabb4 = { aabb.min.x,aabb.min.y,aabb.max.z };
	Vector3 screenaabb4;
	screenaabb4 = Transform(Transform(aabb4, viewProjectionMatrix), viewportMatrix);

	Vector3 aabb5; // maxの左
	aabb5 = { aabb.min.x,aabb.max.y,aabb.max.z };
	Vector3 screenaabb5;
	screenaabb5 = Transform(Transform(aabb5, viewProjectionMatrix), viewportMatrix);

	Vector3 aabb6; // maxの下
	aabb6 = { aabb.max.x,aabb.min.y,aabb.max.z };
	Vector3 screenaabb6;
	screenaabb6 = Transform(Transform(aabb6, viewProjectionMatrix), viewportMatrix);

	Vector3 aabb7; // maxの位置
	aabb7 = { aabb.max.x,aabb.max.y,aabb.max.z };
	Vector3 screenaabb7;
	screenaabb7 = Transform(Transform(aabb7, viewProjectionMatrix), viewportMatrix);
	// min組
	Novice::DrawLine(int(screenaabb0.x), int(screenaabb0.y), int(screenaabb1.x), int(screenaabb1.y), color);
	Novice::DrawLine(int(screenaabb0.x), int(screenaabb0.y), int(screenaabb2.x), int(screenaabb2.y), color);
	Novice::DrawLine(int(screenaabb2.x), int(screenaabb2.y), int(screenaabb3.x), int(screenaabb3.y), color);
	Novice::DrawLine(int(screenaabb1.x), int(screenaabb1.y), int(screenaabb3.x), int(screenaabb3.y), color);
	// min と maxをつなぐ線
	Novice::DrawLine(int(screenaabb0.x), int(screenaabb0.y), int(screenaabb4.x), int(screenaabb4.y), color);
	Novice::DrawLine(int(screenaabb1.x), int(screenaabb1.y), int(screenaabb5.x), int(screenaabb5.y), color);
	Novice::DrawLine(int(screenaabb2.x), int(screenaabb2.y), int(screenaabb6.x), int(screenaabb6.y), color);
	Novice::DrawLine(int(screenaabb3.x), int(screenaabb3.y), int(screenaabb7.x), int(screenaabb7.y), color);
	// max組
	Novice::DrawLine(int(screenaabb4.x), int(screenaabb4.y), int(screenaabb5.x), int(screenaabb5.y), color);
	Novice::DrawLine(int(screenaabb4.x), int(screenaabb4.y), int(screenaabb6.x), int(screenaabb6.y), color);
	Novice::DrawLine(int(screenaabb6.x), int(screenaabb6.y), int(screenaabb7.x), int(screenaabb7.y), color);
	Novice::DrawLine(int(screenaabb5.x), int(screenaabb5.y), int(screenaabb7.x), int(screenaabb7.y), color);

}

void DrawBezier(const Vector3& controlPoints0, const Vector3& controlPoints1, const Vector3& controlPoints2, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	for (int index = 0; index < 32; index++)
	{
		float t = float(index) / 32.0f;
		float nextT = float(index + 1) / 32.0f;
		Vector3 bezierPoint = Bezier(controlPoints0, controlPoints1, controlPoints2, t);
		Vector3 bezierPointNext = Bezier(controlPoints0, controlPoints1, controlPoints2, nextT);
		
		Vector3 bezierPointScreen = Transform(Transform(bezierPoint, viewProjectionMatrix), viewportMatrix);
		Vector3 bezierPointNextScreen = Transform(Transform(bezierPointNext, viewProjectionMatrix), viewportMatrix);

		Novice::DrawLine(
			int(bezierPointScreen.x), int(bezierPointScreen.y),
			int(bezierPointNextScreen.x), int(bezierPointNextScreen.y), color
		);
	}
	
}

bool IsCollision(const AABB& aabb1, const Segment& segment)
{
	float txmin = (aabb1.min.x - segment.origin.x) / segment.diff.x;
	float txmax = (aabb1.max.x - segment.origin.x) / segment.diff.x;

	float tymin = (aabb1.min.y - segment.origin.y) / segment.diff.y;
	float tymax = (aabb1.max.y - segment.origin.y) / segment.diff.y;

	float tzmin = (aabb1.min.z - segment.origin.z) / segment.diff.z;
	float tzmax = (aabb1.max.z - segment.origin.z) / segment.diff.z;

	float tNearX = min(txmin, txmax); float tNearY = min(tymin, tymax); float tNearZ = min(tzmin, tzmax);
	float tFarX = max(txmin, txmax); float tFarY = max(txmin, txmax); float tFarZ = max(tzmin, tzmax);

	float tmin = max(max(tNearX, tNearY), tNearZ);

	float tmax = min(min(tFarX, tFarY), tFarZ);
	if (tmin <= tmax)
	{
		if (tmax >= 0 && tmin <= 1 )
		{
			return true;
		}
	}
	
	return false;
	
}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//描画
	/*Vector3 v1{ 1.2f,-3.9f,2.5f };
	Vector3 v2{ 2.8f,0.4f,-1.3f };
	Vector3 cross = Cross(v1, v2);

	Segment segment{
		.origin{-0.7f,0.3f,0.0f},
		.diff{2.0f,-0.5f,0.0f}
	};

	Vector3 point{ -1.5f,0.6f,0.6f };

	Sphere sphere1;
	sphere1.center = { 0.0f,0.0f,0.0f };
	sphere1.radius = 1;

	Triangle triangle;
	triangle.vertices[0] = { -1.0f,0.0f,0.0f };
	triangle.vertices[1] = { 0.0f,1.0f,0.0f };
	triangle.vertices[2] = { 1.0f,0.0f,0.0f };
	
	Plane plane;
	plane.normal = { 0.0f,1.0f,0.0f };
	plane.distance = 1;

	Vector3 cameraTranslate{ 0.0f,2.9f,-10.49f };

	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };

	AABB aabb1
	{
		.min{-0.5f, -0.5f, -0.5f},
		.max{ 0.5f,  0.5f,  0.5f},
	};

	Vector3 controlPoints[3] = {
		{-0.8f,0.58f,1.0f},
		{1.76f,1.0f,-0.3f},
		{0.94f,-0.7f,2.3f},
	};

	Vector3 translates[3] =
	{
		{0.2f,1.0f,0.0f},
		{0.4f,0.0f,0.0f},
		{0.3f,0.0f,0.0f},
	};

	Vector3 rotates[3] =
	{
		{0.0f,0.0f,-6.8f},
		{0.0f,0.0f,-1.4f},
		{0.0f,0.0f,0.0f},
	};

	Vector3 scales[3] =
	{
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f},
	};*/

	Vector3 a{ 0.2f,1.0f,0.0f };
	Vector3 b{ 2.4f,3.1f,1.2f };
	Vector3 c = a + b;
	Vector3 d = a - b;
	Vector3 e = a * 2.4f;
	Vector3 rotate{ 0.4f,1.43f,-0.8f };
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateMatrix = rotateXMatrix * rotateYMatrix * rotateZMatrix;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		// 描画
		/*Vector3 project = Project(Subtract(point, segment.origin), segment.diff);
		Vector3 closestPoint = ClosestPoint(point, segment);*/

		///
		/// ↓更新処理ここから
		///

		// 描画処理
		/*
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);

		Matrix4x4 viewMatrix = Inverse(cameraMatrix);

		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, 1280.0f / 720.0f, 0.1f, 100.0f);

		Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);

		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, 1280.0f, 720.0f, 0.0f, 1.0f);

		Sphere pointSphere{ point,0.01f };

		Sphere controlPointsSphere0{ controlPoints[0],0.01f};
		Sphere controlPointsSphere1{ controlPoints[1],0.01f};
		Sphere controlPointsSphere2{ controlPoints[2],0.01f};

		Vector3 start = Transform(Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(Add(segment.origin,segment.diff), viewProjectionMatrix), viewportMatrix);

		Matrix4x4 localShoulder = MakeAffineMatrix(scales[0], rotates[0], translates[0]);

		Matrix4x4 localElbow = MakeAffineMatrix(scales[1], rotates[1], translates[1]);

		Matrix4x4 localHand = MakeAffineMatrix(scales[2], rotates[2], translates[2]);

		Matrix4x4 worldShoulder = localShoulder;

		Matrix4x4 worldElbow = Multiply(localElbow, worldShoulder);

		Matrix4x4 worldHand = Multiply(localHand, worldElbow);

		Sphere shoulderCenter =
		{
			{worldShoulder.m[3][0],worldShoulder.m[3][1],worldShoulder.m[3][2]},
			0.1f,
		};

		Sphere elbowCenter =
		{
			{worldElbow.m[3][0],worldElbow.m[3][1],worldElbow.m[3][2]},
			0.1f,
		};

		Sphere handCenter =
		{
			{worldHand.m[3][0],worldHand.m[3][1],worldHand.m[3][2]},
			0.1f,
		};

		Vector3 worldShoulderCenter= Transform(Transform(shoulderCenter.center, viewProjectionMatrix), viewportMatrix);
		Vector3 worldElbowCenter = Transform(Transform(elbowCenter.center, viewProjectionMatrix), viewportMatrix);
		Vector3 worldHandCenter = Transform(Transform(handCenter.center, viewProjectionMatrix), viewportMatrix);

		*/



		ImGui::Begin("Window");

		// カメラ
		//ImGui::DragFloat3("cameraTranslate", &cameraTranslate.x, 0.01f);
		//ImGui::DragFloat3("cameraRotate", &cameraRotate.x, 0.01f);

		ImGui::Text("c:%f,%f,%f", c.x, c.y, c.z);
		ImGui::Text("d:%f,%f,%f", d.x, d.y, d.z);
		ImGui::Text("e:%f,%f,%f", e.x, e.y, e.z);
		ImGui::Text("matrix:\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f.%f\n%f,%f,%f,%f\n",
			rotateMatrix.m[0][0], rotateMatrix.m[0][1], rotateMatrix.m[0][2], rotateMatrix.m[0][3],
			rotateMatrix.m[1][0], rotateMatrix.m[1][1], rotateMatrix.m[1][2], rotateMatrix.m[1][3],
			rotateMatrix.m[2][0], rotateMatrix.m[2][1], rotateMatrix.m[2][2], rotateMatrix.m[2][3],
			rotateMatrix.m[3][0], rotateMatrix.m[3][1], rotateMatrix.m[3][2], rotateMatrix.m[3][3]
		);

		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//描画
		/*
		DrawGrid(viewProjectionMatrix, viewportMatrix);

		//
		DrawSphere(shoulderCenter, viewProjectionMatrix, viewportMatrix, RED);
		DrawSphere(elbowCenter, viewProjectionMatrix, viewportMatrix, GREEN);
		DrawSphere(handCenter, viewProjectionMatrix, viewportMatrix, BLUE);
		
		Novice::DrawLine(int(worldShoulderCenter.x), int(worldShoulderCenter.y), int(worldElbowCenter.x), int(worldElbowCenter.y), WHITE);
		Novice::DrawLine(int(worldElbowCenter.x), int(worldElbowCenter.y), int(worldHandCenter.x), int(worldHandCenter.y), WHITE);

		*/


		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
