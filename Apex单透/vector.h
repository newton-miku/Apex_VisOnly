#pragma once
#define _VECTOR_H
#include <cmath>
#include <stdint.h>

inline float rad2deg(float rad) {
	return rad * 180.0f / 3.1415927f;
}
inline float deg2rad(float deg) {
	return deg * 3.1415927f / 180.0f;
}
class Vector3{
public:

	float x, y, z;

	operator bool() const
	{
		return this->x || this->y || this->z;
	}


	Vector3& operator+=(const Vector3& v)
	{
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
		return *this;
	}

	Vector3 operator-(const Vector3& v) const
	{
		return Vector3{ this->x - v.x, this->y - v.y, this->z - v.z };
	}

	Vector3 operator+(const Vector3& v) const
	{
		return Vector3{ this->x + v.x, this->y + v.y, this->z + v.z };
	}

	Vector3 operator*(const Vector3& v) const
	{
		return Vector3{ this->x * v.x, this->y * v.y, this->z * v.z };
	}

	Vector3 operator*(float mul) const
	{
		return Vector3{ this->x * mul, this->y * mul, this->z * mul };
	}

	Vector3 operator/(float div) const
	{
		return Vector3{ this->x / div, this->y / div, this->z / div };
	}
	/*Vector3& operator=(const Vector3& v)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
		return *this;
	}*/

	float Dot(const Vector3& v) const
	{
		return this->x * v.x + this->y * v.y + this->z * v.z;
	}

	float Distance(const Vector3& v) const
	{
		float x = pow(this->x - v.x, 2);
		float y = pow(this->y - v.y, 2);
		float z = pow(this->z - v.z, 2);

		return sqrtf(x + y + z);
	}
	float Distance2D(const Vector3& v) const
	{
		float x = pow(this->x - v.x, 2);
		float y = pow(this->y - v.y, 2);

		return sqrtf(x + y);
	}
	inline static float distance(Vector3 lhs, Vector3 rhs) {
		Vector3 delta = Vector3{ rhs.x - lhs.x, rhs.y - lhs.y, rhs.z - lhs.z };
		return sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
	}
	inline Vector3 to_angles() const {
		float yaw, pitch;
		if (y == 0.0f && x == 0.0f) {
			yaw = 0.0f;
			pitch = z > 0.0f ? 270.0f : 90.0f;
		}
		else {
			yaw = rad2deg(atan2(y, x));
			const float tmp = sqrt(x * x + y * y);
			pitch = rad2deg(atan2(-z, tmp));
		}
		return Vector3{ pitch, yaw, 0.0f };
	}
	inline Vector3 norm_angles() const {
		const float pitch = x < -90.0f ? -90.0f : (x > 90.0f ? 90.0f : x);
		float yaw = y;
		while (yaw <= -180.0f) yaw += 360.0f;
		while (yaw > 180.0f) yaw -= 360.0f;
		return Vector3{ pitch, yaw, 0.0f };
	}
};

union FloatInt {
	float f32;
	int32_t i32;
	uint32_t u32;
	uint8_t bytes[4];
};
/**/

class Vector2{
public:
	operator bool() const
	{
		return this->x || this->y;
	}

	Vector2& operator-=(const Vector2& v)
	{
		this->x += v.x;
		this->y += v.y;
		return *this;
	}

	Vector2 operator*(float mul) const
	{
		return Vector2{ this->x * mul, this->y * mul };
	}

	float length() const
	{
		return std::sqrt(this->x * this->x + this->y * this->y);
	}

	float x, y;
};