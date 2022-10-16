#include <cmath>
/*
inline float rad2deg(float rad) {
	return rad * 180.0f / 3.1415927f;
}
inline float deg2rad(float deg) {
	return deg * 3.1415927f / 180.0f;
}
struct Vector3 {
	float x, y, z;

	inline static float distance(Vector3 lhs, Vector3 rhs) {
		Vector3 delta = Vector3{ rhs.x - lhs.x, rhs.y - lhs.y, rhs.z - lhs.z };
		return sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
	}
	inline Vector3 operator+ (Vector3 v) const {
		return Vector3{ x + v.x, y + v.y, z + v.z };
	}
	inline Vector3 operator- (Vector3 v) const {
		return Vector3{ x - v.x, y - v.y, z - v.z };
	}
	inline Vector3 operator* (float scale) const {
		return Vector3{ x * scale, y * scale, z * scale };
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
/**/