#include <cmath>

class Vector3 {
public:
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

	float Dot(const Vector3& v) const
	{
		return this->x * v.x + this->y * v.y + this->z * v.z;
	}

	float Distance(const Vector3& v) const
	{
		float x = this->x - v.x;
		float y = this->y - v.y;
		float z = this->z - v.z;

		return sqrtf((x * x) + (y * y) + (z * z));
	}

	float x, y, z;
};

class Vector2 {
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